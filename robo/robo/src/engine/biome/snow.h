#ifndef SNOW_HEADER
#define SNOW_HEADER

#include "biome.h"
#include "../primitives/vertex.h"

class SnowBiome final
	: public Biome
{
public:
	explicit SnowBiome(float maxHeight)
		: m_maxHeight(maxHeight)
	{
		CalculateElementPosition();
		GetColorOfElements();
	}
public:
	biome_t BiomeType(void) override
	{
		return Biome::SNOW;
	}
	glm::vec3 Color(float y) override
	{
		if (y <= m_rock)					return m_snowColor;
		else if (y > m_rock && y <= m_ice)  return m_rockColor;
		else if (y > m_ice)					return m_iceColor;
		else								return glm::vec3(1.0f, 1.0f, 1.0f);
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
				unsigned int grass = rand() % 100;
				if (grass == 53)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row;
					indices[2] = (col + 1) * (ht + 1) + row + 1;

					if (isBelow(m_rock, y, indices)) VaryMeshTriangleColor(indices, v, m_grassColor);
				}

				grass = rand() % 100;
				if (grass == 53)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row + 1;
					indices[2] = col * (ht + 1) + row + 1;

					if (isBelow(m_rock, y, indices)) VaryMeshTriangleColor(indices, v, m_grassColor);
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

	}
protected:
	void CalculateElementPosition(void) override
	{
		m_rock = m_maxHeight * 0.5f;
		m_ice = m_maxHeight * 0.6f;
	}
	void GetColorOfElements(void) override
	{
		m_snowColor = glm::vec3(0.6f, 0.6f, 0.6f);
		m_rockColor = glm::vec3(0.2f, 0.2f, 0.2f);
		m_iceColor = glm::vec3(0.560784f, 0.560784f, 0.737255f);
		m_grassColor = glm::vec3(0.4f, 0.45f, 0.5f);
	}
	void VaryMeshTriangleColor(unsigned int ind[3], Vertex* v, glm::vec3 color)
	{
		v[ind[0]].color = color;
		v[ind[1]].color = color;
		v[ind[2]].color = color;
	}
private:
	float m_rock;
	float m_ice;

	glm::vec3 m_grassColor;
	glm::vec3 m_snowColor;
	glm::vec3 m_rockColor;
	glm::vec3 m_iceColor;

	float m_maxHeight;
};

#endif
