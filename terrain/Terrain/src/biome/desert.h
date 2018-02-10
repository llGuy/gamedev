#ifndef DESERT_HEADER
#define DESERT_HEADER

#include "../primitives/vertex.h"

#include "biome.h"

class DesertBiome
	: public Biome
{
public:
	explicit DesertBiome(float maxHeight)
		: m_maxHeight(maxHeight)
	{
		CalculateElementPosition();
		GetColorOfElements();
	}
public:
	glm::vec3 Color(float y) override
	{
		return m_sandColor;
	}
	void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) override
	{
		srand(time(NULL));
		unsigned int index = 0;
		for (unsigned int col = 0; col < ht; ++col)
		{
			for (unsigned int row = 0; row < wt; ++row)
			{
				unsigned int varying = rand() % 100;
				if (varying == 53)
				{
					unsigned int index1 = col * (ht + 1) + row;
					unsigned int index2 = (col + 1) * (ht + 1) + row;
					unsigned int index3 = (col + 1) * (ht + 1) + row + 1;
					
					v[index1].color = m_varyingSandColor;
					v[index2].color = m_varyingSandColor;
					v[index3].color = m_varyingSandColor;
				}

				varying = rand() % 100;
				if (varying == 53)
				{
					unsigned int index4 = col * (ht + 1) + row;
					unsigned int index5 = (col + 1) * (ht + 1) + row + 1;
					unsigned int index6 = col * (ht + 1) + row + 1;

					v[index4].color = m_varyingSandColor;
					v[index5].color = m_varyingSandColor;
					v[index6].color = m_varyingSandColor;
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
	}
	void GetColorOfElements(void) override
	{
		m_sandColor = glm::vec3(1.0f, 0.97, 0.86f) * 0.6f;
		
		m_varyingSandColor = glm::vec3(0.94, 0.87f, 0.7f) * 0.6f;
	}
private:
	glm::vec3 m_sandColor;
	glm::vec3 m_varyingSandColor;

	float m_maxHeight;
};

#endif