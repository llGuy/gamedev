#ifndef CVEC2_HEADER
#define CVEC2_HEADER

#include <glm/glm.hpp>
#include <unordered_map>

/* generic 2D x-z vector */
struct CVec2
{
	uint8_t x, z;
};

/* x-z vector for world positions */
struct WVec2
{
	int32_t x, z;
	bool operator==(const WVec2& v) const
	{ return (x == v.x && z == v.z); }
};

struct CCorners
{
	glm::vec2 nn;
	glm::vec2 np;
	glm::vec2 pn;
	glm::vec2 pp;
};

#endif