#ifndef DESERT_HEADER
#define DESERT_HEADER

#include "../primitives/vertex.h"

#include "biome.h"

class DesertBiome final
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
	__forceinline biome_t BiomeType(void) override
	{
		return Biome::DESERT;
	}
	__forceinline glm::vec3 Color(float y) override
	{
		return m_sandColor;
	}
	void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) override
	{
		srand(static_cast<int>(time(NULL)));
		unsigned int index = 0;
		unsigned int indices[3];
		for (unsigned int col = 0; col < ht; ++col)
		{
			for (unsigned int row = 0; row < wt; ++row)
			{
				// the second triangle of the grid square
				unsigned int varying = rand() % 100;
				if (varying == 53)
				{
					// abstract this algorithm into the mesh class
					
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row;
					indices[2] = (col + 1) * (ht + 1) + row + 1;
					VaryMeshTriangleColor(indices, v, m_varyingSandColor);
				}

				// the first triangle of the mesh grid square
				varying = rand() % 100;
				if (varying == 53)
				{
					indices[0] = col * (ht + 1) + row;
					indices[1] = (col + 1) * (ht + 1) + row + 1;
					indices[2] = col * (ht + 1) + row + 1;
					VaryMeshTriangleColor(indices, v, m_varyingSandColor);
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
		// does nothing
	}
protected:
	void CalculateElementPosition(void) override
	{
	}
	void GetColorOfElements(void) override
	{
		m_sandColor = glm::vec3(1.0f, 0.97, 0.86f) * 0.7f;
		m_varyingSandColor = glm::vec3(0.94, 0.87f, 0.7f) * 0.7f;
	}
	void VaryMeshTriangleColor(unsigned int ind[3], Vertex* v, glm::vec3 color)
	{
		v[ind[0]].color = color;
		v[ind[1]].color = color;
		v[ind[2]].color = color;
	}
private:
	glm::vec3 m_sandColor;
	glm::vec3 m_varyingSandColor;

	float m_maxHeight;
};

#endif