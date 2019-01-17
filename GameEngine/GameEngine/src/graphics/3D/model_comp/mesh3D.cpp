#include "mesh3D.h"
#include "../model_handler.h"

auto mesh3D_model_computation::compute(object<model_data> & obj) -> void 
{
    auto vertices = create_vertices();

	buffer vertex_buffer;
	vertex_buffer.create();
	vertex_buffer.fill(vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
	attribute vertex_attribute{ 0, GL_FLOAT, 2, GL_FALSE, 2 * sizeof(f32), nullptr };
	
	auto indices = create_indices();

	buffer index_buffer;
	index_buffer.create();
	index_buffer.fill(indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

	obj->vao.create();
	obj->vao.bind();

	obj->vao.attach(vertex_buffer, vertex_attribute);

	component<vertex_buffer_component, model_data> vcomp{ vertex_buffer_component{ vertex_buffer } };
	component<index_buffer_component, model_data> icomp{ index_buffer_component{ index_buffer } };
	obj.add_component(vcomp);
	obj.add_component(icomp);

	obj->count = indices.size();
	obj->mode = GL_TRIANGLE_FAN;
}

auto mesh3D_model_computation::create_vertices(void) -> std::vector<glm::vec2>
{
	assert(x % 2 == 1 && z % 2 == 1);

    std::size_t size = x * z;

    std::vector<glm::vec2> vertices;
    vertices.resize(size);
    
    for (u32 current_z = 0; current_z < z; ++current_z)
	{
		for (u32 current_x = 0; current_x < x; ++current_x)
		{
			u32 index = get_index(current_x, current_z);

			vertices[index] = glm::vec2(static_cast<f32>(current_x), static_cast<f32>(current_z)) - glm::vec2(x, z) / 2.0f;
		}
	}

	return vertices;
}

auto mesh3D_model_computation::create_indices(void) -> std::vector<u32>
{
	constexpr u32 magic = 0xffffffff;

	std::vector<u32> indices;
	indices.reserve( (((x - 1) / 2) * ((z - 1)) / 2) * 10 );

	for (u32 current_z = 1; current_z < z; current_z += 2)
	{
		for (u32 current_x = 1; current_x < z; current_x += 2)
		{
			indices.push_back( get_index(current_x, current_z) );
			indices.push_back( get_index(current_x - 1, current_z - 1) );
			indices.push_back( get_index(current_x - 1, current_z) );
			indices.push_back( get_index(current_x - 1, current_z + 1) );
			indices.push_back( get_index(current_x, current_z + 1) );
			indices.push_back( get_index(current_x + 1, current_z + 1) );
			indices.push_back( get_index(current_x + 1, current_z) );
			indices.push_back( get_index(current_x + 1, current_z - 1) );
			indices.push_back( get_index(current_x, current_z - 1) );
			indices.push_back( get_index(current_x - 1, current_z - 1) );
			indices.push_back( 0xffffffff /* PRIMITIVE RESTART MAGIC # */ );
		}
	}

	return indices;
}