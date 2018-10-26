#include "batch_renderer2D.h"

#define BATCH_SIZE_DEFAULT 100
#define BATCH_SIZE_DEFAULT_BYTES sizeof(renderable2D) * BATCH_SIZE_DEFAULT

auto batch_renderer2D::init(void) -> void
{
	count = 0;

	batch_buffer.create();
	batch_buffer.fill<void>(BATCH_SIZE_DEFAULT_BYTES, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);

	u32 indices[BATCH_SIZE_DEFAULT];
	for (u32 i = 0, off = 0; i < BATCH_SIZE_DEFAULT; i += 6, off += 4)
	{
		indices[i] = off;
		indices[i + 1] = off + 1;
		indices[i + 2] = off + 2;
		indices[i + 3] = off + 2;
		indices[i + 4] = off + 1;
		indices[i + 5] = off + 3;
	}

	index_buffer.create();
	index_buffer.fill(sizeof(indices), indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

	vao.create();
	vao.bind();

	attribute vertex_attribute{ 0, GL_FLOAT, 2, GL_FALSE, sizeof(renderable2D), nullptr };
	attribute uv_attribute{ 1, GL_FLOAT, 2, GL_FALSE, sizeof(renderable2D), (void *)sizeof(glm::vec2) };
	attribute color_attribute{ 2, GL_FLOAT, 4, GL_FALSE, sizeof(renderable2D), (void *)(sizeof(glm::vec2) * 2) };
	attribute tid_attribute{ 3, GL_INT, 1, GL_FALSE, sizeof(renderable2D), (void *)(sizeof(renderable2D_vertex) * 4), 1 };

	vao.attach(batch_buffer, vertex_attribute, uv_attribute, color_attribute, tid_attribute);
}

auto batch_renderer2D::submit(renderable2D & renderable) -> void
{
	batch_buffer.partial_fill(count * sizeof(renderable2D), sizeof(renderable2D), &renderable, GL_ARRAY_BUFFER);
}

auto batch_renderer2D::render(void) -> void
{
	vao.bind();

	index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

auto batch_renderer2D::flush(void) -> void
{
	count = 0;
}