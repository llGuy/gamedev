#include "renderable2D.h"

auto renderable2D::set_position(glm::vec2 const & position, glm::vec2 const & size) -> void
{
	vertices[0].position = position;
	(vertices[1].position = position).y -= size.y;
	(vertices[2].position = position).x += size.x;
	vertices[3].position = position + glm::vec2(size.x, -size.y);
}

auto renderable2D::set_uvs(glm::vec2 const & position, glm::vec2 const & size) -> void
{
	vertices[0].uv = position;
	(vertices[1].uv = position).y -= size.y;
	(vertices[2].uv = position).x += size.x;
	vertices[3].uv = position + glm::vec2(size.x, -size.y);
}

auto renderable2D::set_colors(glm::ivec4 const & colors) -> void
{
	auto get_color = [](u32 color) -> glm::vec4
	{
		f32 r = static_cast<f32>(((color >> 24) & 0xff)) / 255.0f;
		f32 g = static_cast<f32>(((color >> 16) & 0xff)) / 255.0f;
		f32 b = static_cast<f32>(((color >> 8)  & 0xff)) / 255.0f;
		f32 a = static_cast<f32>(((color)       & 0xff)) / 255.0f;

		return glm::vec4(r, g, b, a);
	};

	vertices[0].color = get_color(colors[0]);
	vertices[1].color = get_color(colors[1]);
	vertices[2].color = get_color(colors[2]);
	vertices[3].color = get_color(colors[3]);
}

auto renderable2D::get_texture_id(void) -> i32 &
{
	return texture_id;
}

auto renderable2D::set_alpha_negative(void) -> void
{
	for (auto & vertex : vertices)
	{
		vertex.color.a = -1.0f;
	}
}