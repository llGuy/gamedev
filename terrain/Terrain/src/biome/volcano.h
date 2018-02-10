#ifndef VOLCANO_HEADER
#define VOLCANO_HEADER

#include "biome.h"

class VolcanoBiome
	: public Biome
{
public:
	explicit VolcanoBiome(float maxHeight)
		: m_maxHeight(maxHeight)
	{
		CalculateElementPosition();
		GetColorOfElements();
	}
public:
	glm::vec3 Color(float y) override
	{
		return m_obsidianColor;
	}
	void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) override
	{
		srand(time(NULL));
		unsigned int index = 0;
		for (unsigned int col = 0; col < ht; ++col)
		{
			for (unsigned int row = 0; row < wt; ++row)
			{
				unsigned int varying = rand() % 10;
				if (varying == 0)
				{
					unsigned int index1 = col * (ht + 1) + row;
					unsigned int index2 = (col + 1) * (ht + 1) + row;
					unsigned int index3 = (col + 1) * (ht + 1) + row + 1;

					v[index1].color = m_varyingObsidianColor;
					v[index2].color = m_varyingObsidianColor;
					v[index3].color = m_varyingObsidianColor;
				}

				varying = rand() % 10;
				if (varying == 0)
				{
					unsigned int index4 = col * (ht + 1) + row;
					unsigned int index5 = (col + 1) * (ht + 1) + row + 1;
					unsigned int index6 = col * (ht + 1) + row + 1;

					v[index4].color = m_varyingObsidianColor;
					v[index5].color = m_varyingObsidianColor;
					v[index6].color = m_varyingObsidianColor;
				}
			}
		}
	}
	glm::vec3 Sky(void) override
	{
		return glm::vec3(0.2f, 0.2f, 0.2f);
	}
protected:
	void CalculateElementPosition(void) override
	{
		//m_lava = m_maxHeight * 0.4f;
	}
	void GetColorOfElements(void) override
	{
		m_obsidianColor = glm::vec3(0.0f, 0.0f, 0.05f);
		//m_lavaColor = glm::vec3(1.0f, 0.51f, 0.0f) * 0.6f;

		m_varyingObsidianColor = glm::vec3(0.1f, 0.0f, 0.2f) * 0.5f;
		//m_varyingLavaColor = glm::vec3(1.0f, 0.2f, 0.0f) * 0.5f;
	}
private:
	//float m_lava;

	glm::vec3 m_varyingObsidianColor;
	//glm::vec3 m_varyingLavaColor;
	glm::vec3 m_obsidianColor;
	//glm::vec3 m_lavaColor;

	float m_maxHeight;
};

#endif