#pragma once

#include <vector>
#include "../types.h"
#include <glm/glm.hpp>

#include "../buffer.h"
#include "../renderable.h"

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
	auto create_vertices(u32 w, u32 d) -> void
	{
		for (i32 zi = -d / 2; zi < d / 2; ++zi)
		{
			for (i32 xi = -w / 2; xi < w / 2; ++xi)
			{
				f32 x = static_cast<f32>(xi);
				f32 z = static_cast<f32>(zi);

				vertices.push_back(mesh_vertex{ glm::vec3(x, 0.0f, z), glm::vec3() });
			}
		}
	}
};