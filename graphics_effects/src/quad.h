#ifndef _QUAD_H_
#define _QUAD_H_

#include "renderable_3D.h"
#include <glm/glm.hpp>

class quad_3D
	: public renderable_3D
{
public:
	quad_3D(void) = default;

	auto create(void) -> void override;
	auto destroy(void) -> void override;
	auto element_buffer(void) -> buffer & override;
private:
	buffer index_buffer;
	buffer vertex_buffer;
};

#endif