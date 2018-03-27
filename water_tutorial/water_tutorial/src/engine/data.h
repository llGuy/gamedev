#ifndef DATA_HEADER
#define DATA_HEADER

#include <glm/glm.hpp>

struct Locs
{
	uint32_t projLocation;
	uint32_t viewLocation;
};

struct Udata
{
	glm::mat4 projection;
	glm::mat4 view;
};

#endif