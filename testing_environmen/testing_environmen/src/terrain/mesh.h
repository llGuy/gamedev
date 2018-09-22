#pragma once

#include <vector>
#include "../types.h"
#include <glm/glm.hpp>

#include "../buffer.h"
#include "../renderable.h"

#include <GL/glew.h>

class mesh
{
private:
	struct mesh_vertex 
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	std::vector<u32> indices;
	std::vector<mesh_vertex> vertices;
	buffer vertex_buffer;
	buffer index_buffer;

	u32 width, depth;
public:
	auto create(u32 w, u32 d) -> void
	{
		width = w;
		depth = d;
	}

	auto operator[](u32 index) -> mesh_vertex *
	{
		return &vertices[index];
	}
private:
	template <typename T> auto index_mesh_space(T x, T z) -> u32
	{
		u32 xui32 = static_cast<u32>(x);
		u32 zui32 = static_cast<u32>(z);
		return xui32 + Width * zui32;
	}

	auto create_vertices(u32 w, u32 d) -> void
	{
		for (i32 zi = -d / 2; zi < d / 2; ++zi)
		{
			for (i32 xi = -w / 2; xi < w / 2; ++xi)
			{
				f32 x = static_cast<f32>(xi);
				f32 z = static_cast<f32>(zi);

				vertices.push_back(mesh_vertex{ glm::vec3(x, 0.0f, z), glm::vec3(0.6, 0.3f, 0.0f) });
			}
		}
		vertex_buffer.create();
		vertex_buffer.fill(vertices.size() * sizeof(mesh_vertex), vertices.data(), GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
	}
	auto create_indices(u32 w, u32 d) -> void
	{
		u32 index = 0;

		for (u32 x = 0; x < w; ++x)
		{
			for (u32 z = 0; z < d; ++z)
			{
				indices[index++] = index_mesh_space(x, z);
				indices[index++] = index_mesh_space(x + 1, z);
				indices[index++] = index_mesh_space(x + 1, z + 1);

				indices[index++] = index_mesh_space(x, z);
				indices[index++] = index_mesh_space(x + 1, z + 1);
				indices[index++] = index_mesh_space(x, z + 1);
			}
		}
	}
};