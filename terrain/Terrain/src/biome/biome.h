#ifndef BIOME_HEADER
#define BIOME_HEADER

#include <ctime>
#include <glm/glm.hpp>

// element = material like rock, ice, snow, lava, obsidian

class Vertex;
class Biome
{
public:
	enum biome_t
	{
		SNOW,

		DESERT,

		PLANES,

		VOLCANO
	};
	virtual glm::vec3 Color(float y) = 0;
	virtual void VaryColors(Vertex* v, float* y, unsigned int wt, unsigned int ht) = 0;
	virtual glm::vec3 Sky(void) = 0;
protected:
	virtual void CalculateElementPosition(void) = 0;
	virtual void GetColorOfElements(void) = 0;
};

#endif