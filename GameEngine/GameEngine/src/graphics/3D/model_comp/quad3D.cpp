#include "quad3D.h"
#include "../model_handler.h"

auto quad3D_model_computation::compute(object<model_data> & obj) -> void
{
	std::vector<f32> vertices = create_vertices();

	buffer vertex_buffer;
	vertex_buffer.create();
	vertex_buffer.fill(vertices.size() * sizeof(f32), vertices.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	attribute vertex_attribute{ 0, GL_FLOAT, 3, GL_FALSE, 3 * sizeof(f32), nullptr };
	attribute uvs_attribute{ 1, GL_FALSE, 2, GL_FALSE, 2 * sizeof(f32), (void *)(sizeof(f32) * 12) };

	std::vector<u32> indices = create_indices();

	buffer index_buffer;
	index_buffer.create();
	index_buffer.fill(indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

	obj->vao.create();
	obj->vao.bind();

	obj->vao.attach(vertex_buffer, vertex_attribute);

	component<vertex_buffer_component, model_data> vcomp{ vertex_buffer_component{vertex_buffer} };
	component<index_buffer_component, model_data> icomp{ index_buffer_component{index_buffer} };
	obj.add_component(vcomp);
	obj.add_component(icomp);

	obj->count = indices.size();
}

auto quad3D_model_computation::create_vertices(void)->std::vector<f32>
{
	f32 radius = 1.0f;

	return std::vector<f32>
	{
		-radius, -radius, 0.0f,
		-radius, +radius, 0.0f,
		+radius, -radius, 0.0f,
		+radius, +radius, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
}

auto quad3D_model_computation::create_indices(void)->std::vector<u32>
{
	return std::vector<u32>
	{
		0, 1, 2,
		2, 1, 3
	};
}