#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include "buffer.h"
#include "vao.h"
#include "program.h"
#include "texture.h"
#include "resources.h"
#include <glm/glm.hpp>

// use draw arrays
class sky_box
{
public:
	auto create(resource_handler & rh) -> void;
	auto prepare(glm::mat4 & projection) -> void;

	auto vao(void) -> vertex_array &;
	auto count(void) -> uint32_t;
	auto index_buffer(void) -> buffer &;
private:
	auto create_cube(void) -> void;
private:
	static constexpr float cube_size = 100.0f;

	texture sky_box_texture;
	program sky_box_shaders;
	buffer cube_vertices;
	buffer indices;
	vertex_array buffer_layout;
};

#endif