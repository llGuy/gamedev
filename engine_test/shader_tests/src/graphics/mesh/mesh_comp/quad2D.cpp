#include "quad2D.h"
#include "../mesh_handler.h"
#include "../../../api/api.h"

auto quad2D_mesh_computation::compute(u32 mesh_index, mesh_handler & meshes) -> void 
{
	std::vector<f32> vertices = create_vertices();

	buffer vertex_buffer;
	vertex_buffer.create();
	vertex_buffer.fill(vertices.size() * sizeof(f32), vertices.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	attribute vertex_attribute{ 0, GL_FLOAT, 2, GL_FALSE, 4 * sizeof(f32), nullptr };
	attribute texture_attribute{ 1, GL_FLOAT, 2, GL_FALSE, 4 * sizeof(f32), (void *)(sizeof(f32) * 2) };

	vertex_layout layout;
	layout.create();
	layout.bind();

	layout.attach(vertex_buffer, vertex_attribute, texture_attribute);

	meshes.add_component<vertex_buffer_component>(mesh_index, vertex_buffer);

	meshes[mesh_index].count = 4;
	meshes[mesh_index].primitive = GL_TRIANGLE_STRIP;
	meshes[mesh_index].vao = layout;
}

auto quad2D_mesh_computation::create_vertices(void) -> std::vector<f32>
{
	return std::vector<f32>
	{
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, +1.0f, 0.0f, 1.0f,
		+1.0f, -1.0f, 1.0f, 0.0f,
		+1.0f, +1.0f, 1.0f, 1.0f
	};
}