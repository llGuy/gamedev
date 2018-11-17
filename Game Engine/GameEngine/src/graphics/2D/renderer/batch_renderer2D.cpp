#include <array>
#include <numeric>
#include "batch_renderer2D.h"
#include <glm/gtc/type_ptr.hpp>

#define QUADS_MAX 100

auto batch_renderer2D::init(glsl_program * shader) -> void
{
	/* initializing quads cpu-size array */
	quads.reserve(QUADS_MAX);

	/* initializing gpu side quads buffer */
	gpu_batch.create();
	gpu_batch.fill<void>(quads.capacity() * sizeof(quad2D), nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);

	/* initializing vertex attributes for the quads and VAO */
	attribute vertex_attribute{ 0, GL_FLOAT, 2, GL_FALSE, sizeof(vertex2D), nullptr };
	attribute uv_attribute{ 1, GL_FLOAT, 2, GL_FALSE, sizeof(vertex2D), (void *)(sizeof(glm::vec2)) };
	attribute texture_attribute{ 2, GL_FLOAT, 1, GL_FALSE, sizeof(vertex2D), (void *)(sizeof(glm::vec4)) };

	vao.create();
	vao.bind();

	vao.attach(gpu_batch, vertex_attribute, uv_attribute, texture_attribute);

	/* initializing indices on gpu which stay for rest of the runtime of the program */
	u32 indices_data[QUADS_MAX * 6];
	u32 offset_in_vertex_buffer = 0;
	for (u32 i = 0; i < QUADS_MAX * 6; i += 6, offset_in_vertex_buffer += 4)
	{
		indices_data[i] = offset_in_vertex_buffer;
		indices_data[i + 1] = offset_in_vertex_buffer + 1;
		indices_data[i + 2] = offset_in_vertex_buffer + 2;

		indices_data[i + 3] = offset_in_vertex_buffer + 2;
		indices_data[i + 4] = offset_in_vertex_buffer + 1;
		indices_data[i + 5] = offset_in_vertex_buffer + 3;
	}

	indices.create();
	indices.fill(QUADS_MAX * 6 * sizeof(u32), indices_data, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
	
	/* initializing shader texture uniform data */
	std::array<i32, 32> texture_slot_inits;
	std::iota(texture_slot_inits.begin(), texture_slot_inits.end(), 0);

	this->shader = shader;
	this->shader->bind();
	this->shader->send_uniform_intv("diffuse", texture_slot_inits.data(), 32);

	quad2D default_quad;
	default_quad[0] = vertex2D{ glm::vec2(-1.0f, +1.0f), glm::vec2(0.0f, 1.0f), 0 };
	default_quad[1] = vertex2D{ glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f), 0 };
	default_quad[2] = vertex2D{ glm::vec2(+1.0f, +1.0f), glm::vec2(1.0f, 1.0f), 0 };
	default_quad[3] = vertex2D{ glm::vec2(+1.0f, -1.0f), glm::vec2(1.0f, 0.0f), 0 };

	gpu_batch.partial_fill(0, sizeof(quad2D), &default_quad, GL_ARRAY_BUFFER);
}

auto batch_renderer2D::submit(quad2D const & quad,  texture * diffuse) -> void
{
	bool need_to_add_texture = true;

	u32 slot;

	for (u32 i = 0; i < textures.size(); ++i)
	{
		if (textures[i]->get_id() == diffuse->get_id())
		{
			slot = i;
			need_to_add_texture = false;
			break;
		}
	}

	if (need_to_add_texture)
	{
		slot = textures.size();
		textures.push_back(diffuse);
	}

	quad2D q = quad;

	for (u32 i = 0; i < 4; ++i)
	{
		q[i].texture_id = static_cast<f32>(slot);
	}

	gpu_batch.partial_fill(sizeof(quad2D) * (quads.size() + 1), sizeof(quad2D), &q, GL_ARRAY_BUFFER);

	quads.push_back(q);
}

auto batch_renderer2D::render(void) -> void
{
	shader->bind();

	vao.bind();
	indices.bind(GL_ELEMENT_ARRAY_BUFFER);

	for (u32 i = 0; i < textures.size(); ++i)
	{
		textures[i]->bind(GL_TEXTURE_2D, i);
	}

	glDrawElements(GL_TRIANGLES, quads.size() * 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(u32)));

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

auto batch_renderer2D::render_first(glsl_program * ext_shader) -> void
{
	if (ext_shader) ext_shader->bind();
	else shader->bind();

	vao.bind();
	indices.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

auto batch_renderer2D::flush(void) -> void
{
	quads.clear();
	textures.clear();
}