#define STB_IMAGE_IMPLEMENTATION

#include <math.h>
#include <iostream>
#include <stb-master/stb_image.h>

#include "../heightmap/heightmap.h"
#include "../primitives/vertex.h"
#include "terrain.h"

#include "../biome/snow.h"
#include "../biome/volcano.h"
#include "../biome/plains.h"
#include "../biome/desert.h"

#include "../engine.h"

static float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

Terrain::Terrain(Terrain::TerrainDimensions dimensions, Biome::biome_t biome)
	:	m_dimensions(dimensions), 
		m_hm(HM),
		m_buffer()
{
	TerrainInit(biome);
	BufferInit();
}
Terrain::~Terrain(void)
{
	delete[] m_meshData.vertices.vData;
	delete[] m_meshData.indices.iData;
	delete[] m_meshData.y;
}
void Terrain::TerrainInit(Biome::biome_t biome)
{
	GenerateTerrainVerts();
	GenerateTerrainIndices();
	InitBiome(biome);
	GenerateColors();
	m_biome->VaryColors(m_meshData.vertices.vData, m_meshData.y, m_meshData.xNumTiles, m_meshData.zNumTiles);
}
void Terrain::BufferInit(void)
{
	InitBuffer();
}
void Terrain::Draw(Entity::UniData& ud, UniformLocations* locations, Time* time)
{
	m_buffer.BindAll();
	glm::mat4 modelMat = glm::mat4(1.0f);
	SendUniformData(ud, modelMat, locations, time);
	glDrawElements(GL_TRIANGLES, m_meshData.indices.numIndices, GL_UNSIGNED_SHORT, 0);
	m_biome->RenderBiomeElements(ud, time);
}
glm::vec3 Terrain::Sky(void)
{
	return m_biome->Sky();
}
void Terrain::InitializeHeights(void)
{
	m_meshData.y = new float[m_meshData.vertices.numVertices];
	m_hm.ComputeHeights(m_meshData.y, m_meshData.xNumVerts, m_meshData.zNumVerts, m_dimensions.yMax);
}
void Terrain::GenerateTerrainVerts(void)
{
	//computing the halfs
	m_meshData.xHalf = m_dimensions.x / 2;			// make the terrain centered
	m_meshData.zHalf = m_dimensions.z / 2;
	//computing the number of vertices
	m_meshData.xNumVerts = static_cast<unsigned int>(m_dimensions.x + 1);
	m_meshData.zNumVerts = static_cast<unsigned int>(m_dimensions.z + 1);
	//computing the number of squares / tiles
	m_meshData.xNumTiles = m_meshData.xNumVerts - 1;
	m_meshData.zNumTiles = m_meshData.zNumVerts - 1;

	m_meshData.vertices.numVertices = m_meshData.xNumVerts * m_meshData.zNumVerts;
	m_meshData.vertices.vData = new Vertex[m_meshData.vertices.numVertices];
	InitializeHeights();
	for (unsigned int col = 0; col < m_meshData.zNumVerts; ++col)
	{
		for (unsigned int row = 0; row < m_meshData.xNumVerts; ++row)
		{
			unsigned int index = row + m_meshData.zNumVerts * col;		// should be m_meshData.xNumVerts

			Vertex vert;

			float x = row - m_meshData.xHalf;
			float y = m_meshData.y[index];
			float z = col - m_meshData.xHalf;

			vert.pos = glm::vec3(x, y, z);

			m_meshData.vertices.vData[index].pos = vert.pos;
		}
	}
}
void Terrain::GenerateTerrainIndices(void)
{
	auto getNumIndices = [&](void)->unsigned int {
		return ((m_meshData.xNumVerts - 1) * 6) * (m_meshData.zNumVerts - 1); };
	m_meshData.indices.numIndices = getNumIndices();
	unsigned int index = 0;
	m_meshData.indices.iData = new unsigned short[m_meshData.indices.numIndices];

	for (unsigned int col = 0; col < m_meshData.zNumTiles; ++col)
	{
		for (unsigned int row = 0; row < m_meshData.xNumTiles; ++row)
		{
			m_meshData.indices.iData[index++] = col * m_meshData.zNumVerts + row;
			m_meshData.indices.iData[index++] = (col + 1) * m_meshData.zNumVerts + row;
			m_meshData.indices.iData[index++] = (col + 1) * m_meshData.zNumVerts + row + 1;

			m_meshData.indices.iData[index++] = col * m_meshData.zNumVerts + row;
			m_meshData.indices.iData[index++] = (col + 1) * m_meshData.zNumVerts + row + 1;
			m_meshData.indices.iData[index++] = col * m_meshData.zNumVerts + row + 1;
		}
	}
}
void Terrain::GenerateColors(void)
{
	for (unsigned int col = 0; col < m_meshData.zNumTiles; ++col)
	{
		for (unsigned int row = 0; row < m_meshData.xNumTiles; ++row)
		{
			unsigned int index = row + m_meshData.zNumVerts * col;
			m_meshData.vertices.vData[index].color = m_biome->Color(m_meshData.y[index]);
		}
	}
}
float Terrain::GetYPosOfPlayer(float x, float z, float debug)
{
	float height;
	glm::vec2 positionOfPlayerOnTile = GetPositionOfPlayerOnTile(x, z);
	glm::vec2 sizeOfTile = GetSizeOfTile(x, z);
	unsigned int ind1, ind2, ind3;

	if (positionOfPlayerOnTile.x <= 1 + positionOfPlayerOnTile.y)
	{
		ind1 = static_cast<unsigned int>(sizeOfTile.y * m_meshData.zNumVerts + sizeOfTile.x);
		ind2 = static_cast<unsigned int>((sizeOfTile.y + 1) * m_meshData.zNumVerts + sizeOfTile.x);
		ind3 = static_cast<unsigned int>((sizeOfTile.y + 1) * m_meshData.zNumVerts + sizeOfTile.x + 1);

		height = BarryCentric(glm::vec3(0.0f, m_meshData.y[ind1], -1.0f), glm::vec3(0.0f, m_meshData.y[ind2], 0.0f),
			glm::vec3(1.0f, m_meshData.y[ind3], 0.0f), glm::vec2(positionOfPlayerOnTile.x, positionOfPlayerOnTile.y));
	}
	else
	{
		ind1 = static_cast<unsigned int>(sizeOfTile.y * m_meshData.zNumVerts + sizeOfTile.x);
		ind2 = static_cast<unsigned int>((sizeOfTile.y + 1) * m_meshData.zNumVerts + sizeOfTile.x + 1);
		ind3 = static_cast<unsigned int>(sizeOfTile.y * m_meshData.zNumVerts + sizeOfTile.x + 1);

		height = BarryCentric(glm::vec3(0.0f, m_meshData.y[ind1], -1.0f), glm::vec3(1.0f, m_meshData.y[ind2], 0.0f),
			glm::vec3(1.0f, m_meshData.y[ind3], -1.0f), glm::vec2(positionOfPlayerOnTile.x, positionOfPlayerOnTile.y));
	}

	return height;
}
void Terrain::SendUniformData(Entity::UniData& ud, glm::mat4& model, UniformLocations* locations, Time* time)
{
	glUniformMatrix4fv(locations->m_uniLocProjection, 1, GL_FALSE, &ud.projection[0][0]);
	glUniformMatrix4fv(locations->m_uniLocView, 1, GL_FALSE, &ud.view[0][0]);
	glUniformMatrix4fv(locations->m_uniLocModel, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(locations->m_uniLocLightPosition, 1, &ud.lightPosition[0]);
	glUniform3fv(locations->m_uniLocEyePosition, 1, &ud.eyePosition[0]);
	glUniform1f(locations->m_uniLocTime, (float)(time->currentTime - time->beginning).count() / 1000000000);
	m_biome->SendAdditionalUniformData(locations->m_uniLocLavaHeightTopPosition);
}
void Terrain::InitBuffer(void)
{
	m_buffer.Init(m_meshData.vertices, m_meshData.indices);
}
void Terrain::InitBiome(Biome::biome_t biome)
{
	if (biome == Biome::SNOW)
		m_biome = new SnowBiome(m_dimensions.yMax);
	else if (biome == Biome::VOLCANO)
		m_biome = new VolcanoBiome(m_dimensions.yMax, m_meshData.vertices, m_meshData.indices, m_meshData.xNumTiles, m_meshData.zNumTiles);
	else if (biome == Biome::PLAINS)
		m_biome = new PlainsBiome(m_dimensions.yMax, m_meshData.vertices, m_meshData.indices);
	else if (biome == Biome::DESERT)
		m_biome = new DesertBiome(m_dimensions.yMax);
}