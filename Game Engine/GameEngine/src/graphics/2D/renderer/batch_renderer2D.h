#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../../../api/api.h"

struct vertex2D
{
	glm::vec2 pos;
	glm::vec2 uv;
	f32 texture_id;
};

struct quad2D
{
	vertex2D vertices[4];

	auto operator[](u32 index) -> vertex2D &
	{
		return vertices[index];
	}
};

/* used to render guis */
class batch_renderer2D
{
private:
	buffer gpu_batch;
	buffer indices;

	vertex_layout vao;

	std::vector<quad2D> quads;
	std::vector<texture *> textures;

	glsl_program * shader;
public:
	batch_renderer2D(void) = default;
	auto init(glsl_program * shader) -> void;

	auto submit(quad2D const & quad, texture * diffuse) -> void;

	auto render(void) -> void;
	auto render_first(glsl_program * shader = nullptr) -> void;

	auto flush(void) -> void;
};