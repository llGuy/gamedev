#include "gui.h"

auto gui::get_coordinates_relative_to_parent(u32 index) -> glm::vec2
{
	return vertices[index].position.pos;
}

auto gui::get_coordinates_relative_to_display_indexed(u32 index) -> glm::vec2
{
	if (parent == nullptr)
	{
		return get_coordinates_relative_to_parent(index);
	}
	else
	{
		return parent->get_coordinates_relative_to_display(index) + get_coordinates_relative_to_parent(index);
	}
}

auto gui::get_renderer(void) -> batch_renderer2D *
{
	return renderer;
}

auto gui::get_parent_coord(u32 index) -> glm::vec2
{
	return parent->get_coordinates_relative_to_parent(0);
}

auto gui::convert_to_screen_space(f32 display_w, f32 display_h, gui_cache const & pixel_vertices) -> gui_cache
{
	gui_cache cache = pixel_vertices;

	/* convert to 0.0f -> 1.0f from top left */
	cache.position.pos /= glm::vec2(display_w, display_h);
	cache.size.pos /= glm::vec2(display_w, display_h);
	/* convert to 0.0f -> 1.0f from bottom left */
	cache.position.pos.y = 1.0f - cache.position.pos.y;
	/* convert to screen space OpenGL coordinates */
	cache.position.pos = cache.position.pos * 2.0f - glm::vec2(1.0f);

	cache.size.pos *= 2.0f;

	cache.position.uv.y = 1.0f - cache.position.uv.y;

	return cache;
}

auto gui::bind_renderer(batch_renderer2D & renderer) -> void
{
	this->renderer = &renderer;
}

auto gui::add_child(gui * child) -> void
{
	children.push_back(child);
	child->parent = this;
}

auto gui::get_coordinates_relative_to_display(u32 index) -> glm::vec2
{
	if (parent == nullptr)
	{
		return get_coordinates_relative_to_parent(index);
	}
	else
	{
		return parent->get_coordinates_relative_to_display_indexed() + get_coordinates_relative_to_parent(index);
	}
}

auto gui::update(void) -> void
{
	for (auto child : children)
	{
		child->update();
	}
}

auto gui::submit_to_renderer(i32 display_w, i32 display_h) -> void
{
	if (renderer)
	{
		for (u32 i = 0; i < vertices.size(); ++i)
		{
			gui_cache cache = vertices[i];

			glm::vec2 global_pos = get_coordinates_relative_to_display(i);

			cache.position.pos = global_pos;

			auto real_vertices_cache = convert_to_screen_space((f32)display_w, (f32)display_h, cache);
			auto real_vertices_array = convert_cache_to_vertices(real_vertices_cache);
			renderer->submit(real_vertices_array, diffuse);
		}
	}

	for (auto child : children)
	{
		child->submit_to_renderer(display_w, display_h);
	}
}

auto convert_cache_to_vertices(gui_cache const & cache) -> quad2D
{
	quad2D vertices;

	vertices[0].pos = cache.position.pos;
	(vertices[1].pos = cache.position.pos).y -= cache.size.pos.y;
	(vertices[2].pos = cache.position.pos).x += cache.size.pos.x;
	vertices[3].pos = cache.position.pos + glm::vec2(cache.size.pos.x, -cache.size.pos.y);

	vertices[0].uv = cache.position.uv;
	(vertices[1].uv = cache.position.uv).y -= cache.size.uv.y;
	(vertices[2].uv = cache.position.uv).x += cache.size.uv.x;
	vertices[3].uv = cache.position.uv + glm::vec2(cache.size.uv.x, -cache.size.uv.y);

	return vertices;
}