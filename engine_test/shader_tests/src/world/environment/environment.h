#pragma once

#include "../../api/api.h"
#include "../../math/math.h"

/* cube map */
class environment
{
private:
	texture cubemap_texture;

	glm::vec3 translation_matrix;
public:
	auto init_texture(void) -> void;
};