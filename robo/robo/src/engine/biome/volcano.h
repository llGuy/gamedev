#ifndef VOLCANO_HEADER
#define VOLCANO_HEADER

#include "biome.h"

class VolcanoBiome final
	: public Biome
{
private:
	struct UniformLocations
	{
		signed int projectionLoc;
		signed int viewLoc;
		signed int modelLoc;
		signed int eyePosLoc;
		signed int lightPosLoc;
		signed int timeLoc;
	};
public:
	explicit VolcanoBiome(float maxHeight, VertexData vData, IndexData iData, unsigned int wt, unsigned int ht)
		: m_maxHeight(maxHeight),
		m_lavaShprogram("..\\robo\\res\\wavingVsh.shader", "..\\robo\\res\\wavingFsh.shader", "..\\robo\\res\\wavingGsh.shader"),
		m_indexData(iData)
	{
		m_vertexData.vData = new Vertex[vData.numVertices];
		m_vertexData.numVertices = vData.numVertices;
		memcpy(m_vertexData.vData, vData.vData, vData.numVertices * sizeof(Vertex));

		CalculateElementPosition();
		GetColorOfElements();
		SetLavaPosition(wt, ht);
		LinkLavaShaders();
		GetUniformLocations();
		m_buffer.Init(m_vertexData, m_indexData);
	}
public:
	biome_t BiomeType(void) override
	{
		return Biome::VOLCANO;
	}
	glm::vec3 Color(float y) override
	{
		if (y <= m_lava) return m_obsidianColor;
		else return m_lavaColor;
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
				unsigned int varying = rand() % 10;
				
				if (varying == 0)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row;
					indices[2] = (col + 1) * (ht + 1) + row + 1;

					m_vertexData.vData[index].color = m_varyingLavaColor;

					if (isBelow(m_lava, y, indices)) VaryMeshTriangleColor(indices, v, m_varyingObsidianColor);
					else VaryMeshTriangleColor(indices, v, m_varyingLavaColor);
				}

				varying = rand() % 10;
				if (varying == 0)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row + 1;
					indices[2] = col * (ht + 1) + row + 1;

					if (isBelow(m_lava, y, indices)) VaryMeshTriangleColor(indices, v, m_varyingObsidianColor);
					else VaryMeshTriangleColor(indices, v, m_varyingLavaColor);
				}
			}
		}
	}
	glm::vec3 Sky(void) override
	{
		return glm::vec3(0.2f, 0.2f, 0.2f);
	}
	void RenderBiomeElements(Entity::UniData& ud, Time* time) override
	{
		m_buffer.BindAll();
		m_lavaShprogram.UseProgram();
		glm::mat4 model = glm::mat4(1.0f);
		SendUniformData(ud, model, (float)(time->currentTime - time->beginning).count() / 1000000000);
		glDrawElements(GL_TRIANGLES, m_indexData.numIndices, GL_UNSIGNED_SHORT, 0);
	}
	void SendAdditionalUniformData(signed int location) override
	{
		glUniform1f(location, m_lava);
	}
protected:
	void CalculateElementPosition(void) override
	{
		m_lava = m_maxHeight * 0.5f;
	}
	void GetColorOfElements(void) override
	{
		m_obsidianColor = glm::vec3(0.0f, 0.0f, 0.02f);
		m_lavaColor = glm::vec3(1.0f, 0.0f, 0.0f);

		m_varyingObsidianColor = glm::vec3(0.05f, 0.0f, 0.1f) * 0.5f;
		m_varyingLavaColor = glm::vec3(1.0f, 69.0f / 255.0f, 0.0f);
	}
	void LinkLavaShaders(void)
	{
		std::vector<std::string> attribLocations = { "aM_vertexPosition", "aM_vertexColor" };
		m_lavaShprogram.Compile();
		m_lavaShprogram.Link(attribLocations);
	}
	void GetUniformLocations(void)
	{
		m_locations.projectionLoc = glGetUniformLocation(m_lavaShprogram.ProgramID(), "u_projectionMatrix");
		m_locations.viewLoc = glGetUniformLocation(m_lavaShprogram.ProgramID(), "u_viewMatrix");
		m_locations.modelLoc = glGetUniformLocation(m_lavaShprogram.ProgramID(), "u_modelMatrix");
		m_locations.eyePosLoc = glGetUniformLocation(m_lavaShprogram.ProgramID(), "u_eyePosition");
		m_locations.lightPosLoc = glGetUniformLocation(m_lavaShprogram.ProgramID(), "u_lightPosition");
		m_locations.timeLoc = glGetUniformLocation(m_lavaShprogram.ProgramID(), "u_time");
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
	// lava mesh that waves
	void SetLavaPosition(unsigned int wt, unsigned int ht)
	{
		for (unsigned int i = 0; i < m_vertexData.numVertices; ++i)
		{
			m_vertexData.vData[i].pos.y = m_maxHeight * 0.22f;
			m_vertexData.vData[i].color = m_lavaColor * 0.8f;
		}

		unsigned int indices[3];
		for (unsigned int col = 0; col < ht; ++col)
		{
			for (unsigned int row = 0; row < wt; ++row)
			{
				unsigned int varying = rand() % 10;
				if (varying == 0)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row;
					indices[2] = (col + 1) * (ht + 1) + row + 1;
					VaryMeshTriangleColor(indices, m_vertexData.vData, m_varyingLavaColor);
				}
				
				varying = rand() % 10;
				if (varying == 0)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row + 1;
					indices[2] = col * (ht + 1) + row + 1;
					VaryMeshTriangleColor(indices, m_vertexData.vData, m_varyingLavaColor);
				}
			}
		}
	}
	void VaryMeshTriangleColor(unsigned int ind[3], Vertex* v, glm::vec3 color)
	{
		v[ind[0]].color = color;
		v[ind[1]].color = color;
		v[ind[2]].color = color;
	}
private:
	float m_lava;

	glm::vec3 m_varyingObsidianColor;
	glm::vec3 m_varyingLavaColor;

	glm::vec3 m_obsidianColor;
	glm::vec3 m_lavaColor;

	float m_maxHeight;

	VertexData m_vertexData;
	IndexData m_indexData;
	SHProgram m_lavaShprogram;
	Buffer m_buffer;
	UniformLocations m_locations;
};

#endif