#ifndef PLAINS_HEADER
#define PLAINS_HEADER

#include <chrono>

#include "biome.h"

#include "../data/vertex_data.h"
#include "../data/index_data.h"
#include "../../buffer/buffer.h"
#include "../../shader/shprogram.h"

class PlainsBiome final
	: public Biome
{
public:
	struct UniformLocations
	{
		signed int projectionLoc;
		signed int viewLoc;
		signed int modelLoc;
		signed int eyePosLoc;
		signed int lightPosLoc;
		signed int timeLoc;
	};

	explicit PlainsBiome(float maxHeight, VertexData vData, IndexData iData)
		: m_maxHeight(maxHeight), m_indexData(iData),
		m_waterShprogram("..\\robo\\res\\wavingVsh.shader", "..\\robo\\res\\wavingFsh.shader", "..\\robo\\res\\wavingGsh.shader")
	{
		m_vertexData.vData = new Vertex[vData.numVertices];
		m_vertexData.numVertices = vData.numVertices;
		memcpy(m_vertexData.vData, vData.vData, vData.numVertices * sizeof(Vertex));

		SetWaterPostion();
		CalculateElementPosition();
		GetColorOfElements();
		LinkWaterShaders();
		GetUniformLocations();
		m_buffer.Init(m_vertexData, m_indexData);
	}
public:
	biome_t BiomeType(void) override
	{
		return Biome::PLAINS;
	}
	glm::vec3 Color(float y) override
	{
		if (y < m_grass) return m_darkGrassColor;
		else if (y > m_grass && y <= m_rock) return m_grassColor;
		else return m_rockColor;
	}
	void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) override
	{
		auto isBelow = [&](float yValue, float* y, unsigned int indices[3])->const bool
		{
			return (y[indices[0]] <= yValue || y[indices[1]] <= yValue || y[indices[2]] <= yValue);
		};

		srand(static_cast<int>(time(NULL)));
		unsigned int index = 0;
		unsigned int indices[3];
		for (unsigned int col = 0; col < ht; ++col)
		{
			for (unsigned int row = 0; row < wt; ++row)
			{
				unsigned int grass = rand() % 10;
				if (grass == 53)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row;
					indices[2] = (col + 1) * (ht + 1) + row + 1;

					// checking heights
					if (isBelow(m_grass, y, indices)) VaryMeshTriangleColor(indices, v, m_varyingDarkGrassColor);
					if (isBelow(m_rock, y, indices))  VaryMeshTriangleColor(indices, v, m_varyingGrassColor);
				}

				grass = rand() % 100;
				if (grass == 53)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row + 1;
					indices[2] = col * (ht + 1) + row + 1;

					// checking heights
					if (isBelow(m_grass, y, indices)) VaryMeshTriangleColor(indices, v, m_varyingDarkGrassColor);
					if (isBelow(m_rock, y, indices))  VaryMeshTriangleColor(indices, v, m_varyingGrassColor);
				}
			}
		}
	}
	glm::vec3 Sky(void) override
	{
		return glm::vec3(0.0f, 0.5f, 0.9f);
	}
	void RenderBiomeElements(Entity::UniData& ud, Time* time) override
	{
		m_buffer.BindAll();
		m_waterShprogram.UseProgram();
		glm::mat4 model = glm::mat4(1.0f);
		SendUniformData(ud, model, (float)(time->currentTime - time->beginning).count() / 1000000000);
		glDrawElements(GL_TRIANGLES, m_indexData.numIndices, GL_UNSIGNED_SHORT, 0);
	}
protected:
	void CalculateElementPosition(void) override
	{
		m_grass = m_maxHeight * 0.3f;
		m_rock = m_maxHeight * 0.6f;
	}
	void GetColorOfElements(void) override
	{
		m_darkGrassColor = glm::vec3(0.16, 0.8f, 0.13f) * 0.6f;
		m_rockColor = glm::vec3(0.2f, 0.2f, 0.2f);
		m_grassColor = glm::vec3(0.19f, 1.0f, 0.19f) * 0.6f;

		m_varyingGrassColor = glm::vec3(0.48f, 1.0f, 0.0f) * 0.6f;
		m_varyingDarkGrassColor = glm::vec3(0.35f, 0.16f, 0.14f);
	}
	void LinkWaterShaders(void)
	{
		std::vector<std::string> attribLocations = {"aM_vertexPosition", "aM_vertexColor"};
		m_waterShprogram.Compile();
		m_waterShprogram.Link(attribLocations);
	} 
	void GetUniformLocations(void)
	{
		m_locations.projectionLoc = glGetUniformLocation(m_waterShprogram.ProgramID(), "u_projectionMatrix");
		m_locations.viewLoc = glGetUniformLocation(m_waterShprogram.ProgramID(), "u_viewMatrix");
		m_locations.modelLoc = glGetUniformLocation(m_waterShprogram.ProgramID(), "u_modelMatrix");
		m_locations.eyePosLoc = glGetUniformLocation(m_waterShprogram.ProgramID(), "u_eyePosition");
		m_locations.lightPosLoc = glGetUniformLocation(m_waterShprogram.ProgramID(), "u_lightPosition");
		m_locations.timeLoc = glGetUniformLocation(m_waterShprogram.ProgramID(), "u_time");
	}
	void SendUniformData(Entity::UniData& ud, glm::mat4& model, float time)
	{
		glUniformMatrix4fv(m_locations.projectionLoc, 1, GL_FALSE, &ud.projection[0][0]);
		glUniformMatrix4fv(m_locations.viewLoc, 1, GL_FALSE, &ud.view[0][0]);
		glUniformMatrix4fv(m_locations.modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniform3fv(m_locations.eyePosLoc, 1, &ud.eyePosition[0]);
		glUniform3fv(m_locations.lightPosLoc, 1, &ud.lightPosition[0]);
		glUniform1f(m_locations.timeLoc, time);
	}
	void SetWaterPostion(void)
	{
		for (unsigned int i = 0; i < m_vertexData.numVertices; ++i)
		{
			m_vertexData.vData[i].pos.y = m_maxHeight * 0.28f;
			m_vertexData.vData[i].color = glm::vec3(5.0f / 255.0f, 26.0f / 255.0f, 180.0f / 255.0f);
		}
	}
	void VaryMeshTriangleColor(unsigned int ind[3], Vertex* v, glm::vec3 color)
	{
		v[ind[0]].color = color;
		v[ind[1]].color = color;
		v[ind[2]].color = color;
	}
private:
	float m_grass;
	float m_rock;

	glm::vec3 m_varyingDarkGrassColor;
	glm::vec3 m_varyingGrassColor;

	glm::vec3 m_darkGrassColor;
	glm::vec3 m_grassColor;
	glm::vec3 m_rockColor;

	float m_maxHeight;
	
	VertexData m_vertexData;
	IndexData m_indexData;
	SHProgram m_waterShprogram;
	Buffer m_buffer;
	UniformLocations m_locations;
};

#endif