#include "cube.h"
#include "../mesh_handler.h"

auto cube_mesh_computation::operator()(u32 mesh_index, mesh_handler & meshes) -> void
{
	std::vector<f32> vertices = create_vertices();

	buffer vertex_buffer;
	vertex_buffer.create();
	vertex_buffer.fill(vertices.size() * sizeof(f32), vertices.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
	attribute vertex_attribute{ 0, GL_FLOAT, 3, GL_FALSE, 3 * sizeof(f32), nullptr };

	std::vector<u32> indices = create_indices();

	buffer index_buffer;
	index_buffer.create();
	index_buffer.fill(indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

	vertex_layout layout;
	layout.create();
	layout.bind();

	layout.attach(vertex_buffer, vertex_attribute);

	meshes.add_component<vertex_buffer_component>(mesh_index, vertex_buffer);
	meshes.add_component<index_buffer_component>(mesh_index, index_buffer);

	meshes[mesh_index].count = indices.size();
	meshes[mesh_index].vao = layout;
	meshes[mesh_index].primitive = GL_TRIANGLES;
}

auto cube_mesh_computation::create_vertices(void) -> std::vector<f32>
{
	f32 radius = 1.0f;

	return std::vector<f32>
	{
		-radius, -radius, radius,
		radius, -radius, radius,
		radius, radius, radius,
		-radius, radius, radius,

		-radius, -radius, -radius,
		radius, -radius, -radius,
		radius, radius, -radius,
		-radius, radius, -radius
	};
}

auto cube_mesh_computation::create_indices(void)->std::vector<u32>
{
	return std::vector<u32>
	{
		0, 1, 2,
		2, 3, 0,

		1, 5, 6,
		6, 2, 1,

		7, 6, 5,
		5, 4, 7,

		4, 0, 3,
		3, 7, 4,

		4, 5, 1,
		1, 0, 4,

		3, 2, 6,
		6, 7, 3
	};
}