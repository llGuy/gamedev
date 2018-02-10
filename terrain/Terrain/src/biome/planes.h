#ifndef PLANES_HEADER
#define PLANES_HEADER

#include "biome.h"

class PlanesBiome
	: public Biome
{
public:
	explicit PlanesBiome(float maxHeight)
		: m_maxHeight(maxHeight)
	{
		CalculateElementPosition();
		GetColorOfElements();
	}
public:
	glm::vec3 Color(float y) override
	{
		if (y < m_grass)											  
			return m_darkGrassColor;
		else if (y > m_grass && y <= m_rock)  
			return m_grassColor;
		else								  
			return m_rockColor;
	}
	void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) override
	{
		srand(time(NULL));
		unsigned int index = 0;
		for (unsigned int col = 0; col < ht; ++col)
		{
			for (unsigned int row = 0; row < wt; ++row)
			{
				unsigned int grass = rand() % 10;
				if (grass == 53)
				{
					unsigned int index1 = col * (ht + 1) + row;
					unsigned int index2 = (col + 1) * (ht + 1) + row;
					unsigned int index3 = (col + 1) * (ht + 1) + row + 1;

					// checking heights
					if (y[index1] <= m_grass || y[index2] <= m_grass || y[index3] <= m_grass)
					{
						v[index1].color = m_varyingDarkGrassColor;
						v[index2].color = m_varyingDarkGrassColor;
						v[index3].color = m_varyingDarkGrassColor;
					}
					if (y[index1] <= m_rock || y[index2] <= m_rock || y[index3] <= m_rock)
					{
						v[index1].color = m_varyingGrassColor;
						v[index2].color = m_varyingGrassColor;
						v[index3].color = m_varyingGrassColor;
					}
				}

				grass = rand() % 100;
				if (grass == 53)
				{
					unsigned int index4 = col * (ht + 1) + row;
					unsigned int index5 = (col + 1) * (ht + 1) + row + 1;
					unsigned int index6 = col * (ht + 1) + row + 1;

					// checking heights
					if (y[index4] <= m_grass || y[index5] <= m_grass || y[index6] <= m_grass)
					{
						v[index4].color = m_varyingDarkGrassColor;
						v[index5].color = m_varyingDarkGrassColor;
						v[index6].color = m_varyingDarkGrassColor;
					}
					if (y[index4] <= m_rock || y[index5] <= m_rock || y[index6] <= m_rock)
					{
						v[index4].color = m_varyingGrassColor;
						v[index5].color = m_varyingGrassColor;
						v[index6].color = m_varyingGrassColor;
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
		m_grass = m_maxHeight * 0.3f;
		m_rock = m_maxHeight * 0.7f;
	}
	void GetColorOfElements(void) override
	{
		m_darkGrassColor = glm::vec3(0.16, 0.47f, 0.13f) * 0.8f;
		m_rockColor = glm::vec3(0.2f, 0.2f, 0.2f);
		m_grassColor = glm::vec3(0.19f, 0.8f, 0.19f) * 0.6f;

		m_varyingGrassColor = glm::vec3(0.48f, 1.0f, 0.0f) * 0.7f;
		m_varyingDarkGrassColor = glm::vec3(0.35f, 0.16f, 0.14f);
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
};

#endif