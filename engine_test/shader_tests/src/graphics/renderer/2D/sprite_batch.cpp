#include <algorithm>
#include "sprite_batch.h"

sprite_batch_renderer::sprite_batch_renderer(u32 capacity)
{
	vertices.reserve(capacity);

	gpu_batch.create();
	gpu_batch.fill<void>(vertices.capacity() * sizeof(vertex2D), nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);

	attribute vertex_attribute{ 0, GL_FLOAT, 2, GL_FALSE, sizeof(f32) * 4, nullptr };
	attribute texture_attribute{ 1, GL_FLOAT, 2, GL_FALSE, sizeof(f32) * 4, (void *)(sizeof(f32) * 2) };

	layout.create();
	layout.bind();

	layout.attach(gpu_batch, vertex_attribute, texture_attribute);
}

auto sprite_batch_renderer::submit(std::array<vertex2D, 4> const & new_verts) -> void
{
	u32 offset = vertices.size() * sizeof(vertex2D);

	std::array<vertex2D, 6> verts
	{
		new_verts[0], new_verts[1], new_verts[2],
		new_verts[2], new_verts[1], new_verts[3]
	};

	std::copy(verts.begin(), verts.end(), std::back_inserter(vertices));

	gpu_batch.partial_fill(offset, verts.size() * sizeof(vertex2D), verts.data(), GL_ARRAY_BUFFER);
}

auto sprite_batch_renderer::render(glsl_program & program, mesh_handler & mh) -> void
{
	program.bind();

	for (auto & pre_render : pre_renders)
	{
		pre_render.pre_render->prepare(program);
	}

	layout.bind();

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

auto sprite_batch_renderer::flush(void) -> void
{
	vertices.clear();
}