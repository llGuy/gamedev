#pragma once

#include <vector>
#include "renderer_2D.h"

class sprite_batch_renderer : public renderer_2D
{
private:
	buffer gpu_batch;
	vertex_layout layout;
	std::vector<vertex2D> vertices;
public:
	sprite_batch_renderer(u32 capacity);

	auto submit(std::array<vertex2D, 4> const & vertices) -> void;
	auto render(glsl_program & program, mesh_handler & mh) -> void;
	auto flush(void) -> void;
};