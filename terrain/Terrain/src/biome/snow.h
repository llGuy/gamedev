#ifndef SNOW_HEADER
#define SNOW_HEADER

#include "biome.h"
#include "../primitives/vertex.h"

class SnowBiome
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
	glm::vec3 Color(float y) override
	{
		if (y <= m_rock)					return m_snowColor;
		else if (y > m_rock && y <= m_ice)  return m_rockColor;
		else if (y > m_ice)					return m_iceColor;
		else								return glm::vec3(1.0f, 1.0f, 1.0f);
	}
	void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) override
	{
		srand(time(NULL));
		unsigned int index = 0;
		for (unsigned int col = 0; col < ht; ++col)
		{
			for (unsigned int row = 0; row < wt; ++row)
			{
				unsigned int grass = rand() % 100;
				if (grass == 53)
				{
					unsigned int index1 = col * (ht + 1) + row;
					unsigned int index2 = (col + 1) * (ht + 1) + row;
					unsigned int index3 = (col + 1) * (ht + 1) + row + 1;

					// checking heights
					if (y[index1] <= m_rock || y[index2] <= m_rock || y[index3] <= m_rock)
					{
						v[index1].color = m_grassColor;
						v[index2].color = m_grassColor;
						v[index3].color = m_grassColor;
					}
				}

				grass = rand() % 100;
				if (grass == 53)
				{
					unsigned int index4 = col * (ht + 1) + row;
					unsigned int index5 = (col + 1) * (ht + 1) + row + 1;
					unsigned int index6 = col * (ht + 1) + row + 1;

					// checking heights
					if (y[index4] <= m_rock || y[index5] <= m_rock || y[index6] <= m_rock)
					{
						v[index4].color = m_grassColor;
						v[index5].color = m_grassColor;
						v[index6].color = m_grassColor;
					}
				}
			}
		}
	}
	glm::vec3 Sky(void) override
	{
		return glm::vec3(0.0f, 0.5f, 0.9f);
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
