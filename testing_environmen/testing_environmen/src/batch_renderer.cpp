#include "model_handler.h"
#include "batch_renderer.h"
#include <glm/gtc/type_ptr.hpp>

batch_renderer::batch_renderer(void)
	: size(0), max(10)
{
	data_buffer.create();
	data_buffer.fill<void>(max, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
}

auto batch_renderer::prepare_model_vao(vertex_layout & layout) -> void
{
	layout.bind();

	for (u32 i = 0; i < 4; ++i)
	{
		attribute mat_attrib(GL_FLOAT, 4, GL_FALSE, sizeof glm::mat4, 
			(void*)(sizeof(glm::vec4) * i), std::make_optional<i32>(1));

		layout.attach(data_buffer, mat_attrib);
	}

	unbind_vertex_layouts();
}

auto batch_renderer::submit(glm::mat4 const & transform) -> void
{
	if (size != max)
	{
		data_buffer.partial_fill(sizeof(glm::mat4) * size++,
			sizeof(glm::mat4), glm::value_ptr(transform), GL_ARRAY_BUFFER);
	}
}

auto batch_renderer::render_indices(vertex_layout & vao, buffer & indices, GLenum mode, u32 prim_count, u32 index_count) -> void
{
	vao.bind();
	indices.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElementsInstanced(mode, index_count, GL_UNSIGNED_INT, nullptr, prim_count);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

auto batch_renderer::flush(void) -> void
{
	size = 0;
}