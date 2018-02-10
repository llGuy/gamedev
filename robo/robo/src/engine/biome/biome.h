#ifndef BIOME_HEADER
#define BIOME_HEADER

#include <ctime>
#include <glm/glm.hpp>

#include "../data/time.h"
#include "../entities/entity.h"

// element = material like rock, ice, snow, lava, obsidian

struct Vertex;
class Biome
{
public:
	enum biome_t
	{
		SNOW,

		DESERT,

		PLAINS,

		VOLCANO
	};
	virtual biome_t BiomeType(void) = 0;
	virtual glm::vec3 Color(float y) = 0;
	virtual void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) = 0;
	virtual glm::vec3 Sky(void) = 0;
	virtual void RenderBiomeElements(Entity::UniData& ud, Time* time) = 0;
	virtual void SendAdditionalUniformData(signed int location) {}
protected:
	virtual void CalculateElementPosition(void) = 0;
	virtual void GetColorOfElements(void) = 0;
};

#endif