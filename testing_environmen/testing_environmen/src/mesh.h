#pragma once

#include <utility>

#include "model_handler.h"
#include "types.h"
#include <vector>

template <typename T, typename> struct shape;

/* G will be a noise generator */
template <typename G> struct shape <struct mesh, G>
{
	i32 width, dep;

	G generator;

	shape(i32 w, i32 h, G const & generator)
		: width(w), dep(h), generator(generator)
	{
	}

	auto operator()(model_handler & handler, model_data & data, u32 index) const -> void
	{
		std::vector<glm::vec3> vertices = create_vertices();
		std::vector<u32> indices = create_indices();

		buffer vertex_buffer = create_buffer(vertices, GL_ARRAY_BUFFER);
		buffer index_buffer = create_buffer(indices, GL_ELEMENT_ARRAY_BUFFER);

		data.count = indices.size();
		data.primitive = GL_TRIANGLES;

		data.vao.create();
		data.vao.bind();

		attribute vertex_att{ GL_FLOAT, 3, GL_FALSE, sizeof(glm::vec3), nullptr };
		attribute color_att{ GL_FLOAT, 3, GL_FALSE, sizeof(glm::vec3), 
			(void *)(sizeof(glm::vec3) * width * dep), std::optional<i32>(1) };

		data.vao.attach(vertex_buffer, vertex_att, color_att);

		unbind_vertex_layouts();

		handler.add_component<vertex_buffer_component>(index, vertex_buffer);
		handler.add_component<index_buffer_component>(index, index_buffer);
	}
private:
	auto create_vertices(void) const -> std::vector<glm::vec3>
	{
		glm::vec3 two_colors[]{ 
			glm::vec3(13.0f, 174.0f, 4.0f) / 255.0f,
		glm::vec3(103.0f, 179.0f, 0.1f) / 255.0f };

		std::vector<glm::vec3> vertices;
		vertices.reserve(width * dep + (width - 1) * (dep - 1));

		for (i32 zi = 0; zi < dep; ++zi)
		{
			for (i32 xi = 0; xi < width; ++xi)
			{
				f32 x = static_cast<f32>(xi);
				f32 z = static_cast<f32>(zi);

				vertices.push_back(glm::vec3(x, generator(glm::vec2(x, z)), z));
			}
		}

		for (i32 zi = 0; zi < dep - 1; ++zi)
		{
			for (i32 xi = 0; xi < width - 1; ++xi)
			{
				u32 which = (rand() % 10 > 3);

				vertices.push_back(two_colors[which]);
			}
		}

		return vertices;
	}

	template <typename T> auto create_buffer(std::vector<T> & vec, GLenum point) const -> buffer
	{
		buffer buf;
		buf.create();
		buf.fill(sizeof(T) * vec.size(), vec.data(), GL_STATIC_DRAW, point);

		return buf;
	}

	auto create_indices(void) const -> std::vector<u32>
	{
		std::vector<u32> indices;
		indices.reserve((width - 1) * (dep - 1) * 6);

		u32 index = 0;

		for (i32 x = 0; x < width - 1; ++x)
		{
			for (i32 z = 0; z < dep - 1; ++z)
			{
				indices.push_back(index_mesh_space(x, z));
				indices.push_back(index_mesh_space(x + 1, z));
				indices.push_back(index_mesh_space(x + 1, z + 1));

				indices.push_back(index_mesh_space(x, z));
				indices.push_back(index_mesh_space(x + 1, z + 1));
				indices.push_back(index_mesh_space(x, z + 1));
			}
		}

		return indices;
	}

	template <typename T> auto index_mesh_space(T x, T z) const -> u32
	{
		u32 xui32 = static_cast<u32>(x);
		u32 zui32 = static_cast<u32>(z);
		return xui32 + width * zui32;
	}
};