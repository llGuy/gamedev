#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <array>
#include <iostream>
#include <glm/glm.hpp>
#include "detail.h"
#include "renderable_3D.h"

struct terrain_vertex
{
	glm::vec3 position;
	glm::vec2 texture_coords;
};

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
		return std::make_optional <texture *> ( &textures[i] );
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
	auto generate_vertices(void) -> void
	{
		using detail::construct;
		for (uint32_t z = 0; z < Depth; ++z)
		{
			for (uint32_t x = 0; x < Width; ++x)
			{
				glm::vec2 x_z = construct<glm::vec2, float>(x, z);
				uint32_t index = vertex_index(x, z);
				vertices[index].position = glm::vec3(x_z.x, 0.0f, x_z.y);
				vertices[index].texture_coords = glm::vec2(x_z.x / static_cast<float>(Width), x_z.y / static_cast<float>(Depth));
			}
		}
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
		vertex_buffer.fill(vertices.size() * sizeof(terrain_vertex), vertices.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
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
		buffer_layout.push<float>(0, 3, 5 * sizeof(float), nullptr);
		buffer_layout.push<float>(1, 2, 5 * sizeof(float), (void *)(sizeof(float) * 3));
		index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		vertex_buffer.bind(GL_ARRAY_BUFFER);
	}
	auto load_grass_texture(resource_handler & rh) -> void
	{
		using detail::make_array;

		auto images = make_array(rh.load <image>("src/grass_texture_seamless.png"),
					             rh.load <image>("src/path.png"),
					             rh.load <image>("src/grass_flowers.png"),
					             rh.load <image>("src/mud.png"),
					             rh.load <image>("src/blend_map.png"));

		for (uint32_t i = 0; i < 5; ++i)
		{
			textures[i].create();
 			textures[i].bind();
			textures[i].enable_mipmap();
			textures[i].int_param(GL_TEXTURE_WRAP_S, GL_REPEAT);
			textures[i].int_param(GL_TEXTURE_WRAP_T, GL_REPEAT);
			textures[i].float_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			textures[i].float_param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			textures[i].fill(GL_RGBA, images[i].w, images[i].h, GL_RGBA, GL_UNSIGNED_BYTE, images[i].data);
		}
	}
private:
	std::array<terrain_vertex, Width * Depth> vertices;
	std::array<uint16_t, (Width - 1) * (Depth - 1) * 6> indices;

	buffer vertex_buffer;
	buffer index_buffer;

	std::array<texture, 5> textures;
};

#endif