#ifndef _GRASS_H_
#define _GRASS_H_

#include <glm/glm.hpp>
#include "buffer.h"
#include "resources.h"
#include "vao.h"
#include "texture.h"
#include "program.h"

class grass_platform
{
public:
	grass_platform(void) = default;

	auto create(resource_handler & rh) -> void;
	auto prepare(glm::mat4 &, glm::mat4 &, glm::vec3 &, float) -> void;

	auto vao(void) -> vertex_array &;
private:
	buffer vertex_buffer;
	vertex_array buffer_layout;

	texture normals;
	texture grass_texture;

	program grass_shaders;

	glm::mat4 translation;
};

#endif