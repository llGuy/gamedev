#ifndef CVEC2_HEADER
#define CVEC2_HEADER

#include <glm/glm.hpp>
#include <unordered_map>

/* generic 3D x-z vector */
struct CVec2
{
	unsigned char x, z;
};

/* a x-z vector for the chunk coordinates in world position*/
struct WVec2
{
	signed int x, z;
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