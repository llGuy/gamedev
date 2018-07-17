#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <ctime>
#include <new>
#include <array>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#include "detail.h"
#include "renderable_3D.h"

struct terrain_vertex { glm::vec3 p; glm::vec2 t; };

enum terrain_texture
{
	background, r, g, b, blend_map
};

template <uint32_t Width, uint32_t Depth>
class terrain
	: public renderable_3D
{
public:
	terrain(void) = default;

	auto create(resource_handler & resources) -> void override
	{
//		srand(static_cast<int32_t>(time(NULL)));

		generate_vertices();
		generate_indices();

		create_vertex_buffer();
		create_index_buffer();
		create_vertex_array();

		load_grass_texture(resources);
	}
	auto destroy(void) -> void override
	{
		vertex_buffer.destroy();
		index_buffer.destroy();
		buffer_layout.destroy();
	}
	auto element_buffer(void) -> buffer & override
	{
		return index_buffer;
	}

	auto model_texture(uint32_t i = 0) -> std::optional<texture *> override
	{
		return std::make_optional <texture *>(&textures[i]);
	}

	auto bind_textures(void) -> void
	{
		for (uint32_t i = 0; i < 5; ++i)
		{
			textures[i].bind(i);
		}
	}

	static constexpr auto vertex_count(void) -> uint32_t
	{
		return (Width - 1) * (Depth - 1) * 6;
	}
private:
	template <typename T>
	auto vertex_index(T x, T z) -> uint32_t
	{
		uint32_t xui = static_cast<uint32_t>(x);
		uint32_t zui = static_cast<uint32_t>(z);

		return xui + Width * zui;
	}
	using corners = std::array<glm::vec2, 4>;
	auto generate_vertices(void) -> void
	{
		using detail::construct;

		corners noise_vectors;
		corners corner_vectors;

		prepare_height_data(noise_vectors, corner_vectors);

		for (uint32_t z = 0; z < Depth; ++z)
		{
			for (uint32_t x = 0; x < Width; ++x)
			{
				glm::vec2 x_z = construct<glm::vec2, float>(x, z);
				uint32_t index = vertex_index(x, z);
				vertices[index] = glm::vec3(x_z.x, generate_height(x_z, noise_vectors, corner_vectors), x_z.y);
				texture_coords[index]= glm::vec2(x_z.x / static_cast<float>(Width), x_z.y / static_cast<float>(Depth));
			}
		}
	}
	auto generate_height(glm::vec2 const & x_z, corners const & noise, corners const & positions) -> float
	{
		std::array<float, 4> noise_values;
		for (uint32_t i = 0; i < 4; ++i)
		{
			noise_values[i] = (glm::dot(noise[i], x_z - positions[i]));
		}
		// interpolate on x axes
		auto interpolate = [](float a_hor, float b_hor, float dest) -> float
		{
			float factor = (dest - a_hor) / (b_hor - a_hor);
			return factor;
		};

		auto interp = [](glm::vec2 const & a, glm::vec2 const & b, float ref) -> float
		{
			return a.y + ((ref - a.x) / (b.x - a.x)) * (b.y - a.y);
		};


		float noise_interp_x1 = interp(glm::vec2(positions[0].x, noise_values[0]), glm::vec2(positions[1].x, noise_values[1]), x_z.x);
		float noise_interp_x2 = interp(glm::vec2(positions[2].x, noise_values[2]), glm::vec2(positions[3].x, noise_values[3]), x_z.x);

		float final = interp(glm::vec2(positions[2].y, noise_interp_x2), glm::vec2(positions[0].y, noise_interp_x1), x_z.y);

		return final;
	}
	auto prepare_height_data(corners & corner_vectors,
		corners & corner_positions) -> void
	{
		using detail::vec_rand;
		// use perlin noise algorithm
		std::for_each(corner_vectors.begin(), corner_vectors.end(),
			[&](glm::vec2 & v) { v = glm::normalize(vec_rand<float, 2>(100)); });

		float width = static_cast<float>(Width) + 0.5f, depth = static_cast<float>(Depth) + 0.5f;

		new (corner_positions.data()) corners{
			glm::vec2(-0.5f, -0.5f), glm::vec2(width, -0.5f),
				glm::vec2(-0.5f, depth), glm::vec2(width, depth) };
	}
	auto generate_indices(void) -> void
	{
		uint32_t index = 0;
		for (uint32_t gsz = 0; gsz < Depth - 1; ++gsz)
		{
			for (uint32_t gsx = 0; gsx < Width - 1; ++gsx)
			{
				indices[index++] = vertex_index(gsx, gsz);
				indices[index++] = vertex_index(gsx + 1, gsz);
				indices[index++] = vertex_index(gsx + 1, gsz + 1);

				indices[index++] = vertex_index(gsx, gsz);
				indices[index++] = vertex_index(gsx + 1, gsz + 1);
				indices[index++] = vertex_index(gsx, gsz + 1);
			}
		}
	}
	auto create_vertex_buffer(void) -> void
	{
		vertex_buffer.create();
		vertex_buffer.fill<void>(vertices.size() * sizeof(terrain_vertex), 0, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		vertex_buffer.partial_fill(0, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_ARRAY_BUFFER);
		vertex_buffer.partial_fill(vertices.size() * sizeof(glm::vec3), texture_coords.size() * sizeof(glm::vec2), texture_coords.data(), GL_ARRAY_BUFFER);
	}
	auto create_index_buffer(void) -> void
	{
		index_buffer.create();
		index_buffer.fill(indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
	}
	auto create_vertex_array(void) -> void
	{
		buffer_layout.create();
		buffer_layout.bind();
		buffer_layout.enable(0);
		buffer_layout.enable(1);
		buffer_layout.push<float>(0, 3, 3 * sizeof(float), nullptr);
		buffer_layout.push<float>(1, 2, 2 * sizeof(float), (void *)(sizeof(glm::vec3) * vertices.size()));
		index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		vertex_buffer.bind(GL_ARRAY_BUFFER);
	}
	auto load_grass_texture(resource_handler & rh) -> void
	{
#ifdef _win32
		std::string path = "src/";
#else
		std::string path = "";
#endif

		using detail::make_array;

		auto images = make_array(rh.load <image>(path + std::string("grass_texture_seamless.png")),
			rh.load <image>(path + std::string("path.png")),
			rh.load <image>(path + std::string("grass_flowers.png")),
			rh.load <image>(path + std::string("mud.png")),
			rh.load <image>(path + std::string("blend_map.png")));

		for (uint32_t i = 0; i < 5; ++i)
		{
			textures[i].create();
			textures[i].bind();
			textures[i].int_param(GL_TEXTURE_WRAP_S, GL_REPEAT);
			textures[i].int_param(GL_TEXTURE_WRAP_T, GL_REPEAT);
			textures[i].float_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			textures[i].float_param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			textures[i].fill(GL_RGBA, images[i].w, images[i].h, GL_RGBA, GL_UNSIGNED_BYTE, images[i].data);
			textures[i].enable_mipmap();
		}
	}
private:
	std::array<glm::vec3, Width * Depth> vertices;
	std::array<glm::vec2, Width * Depth> texture_coords;
	std::array<uint16_t, (Width - 1) * (Depth - 1) * 6> indices;

	buffer vertex_buffer;
	buffer index_buffer;

	std::array<texture, 5> textures;
};

#endif
