#include "gui.h"

auto gui::get_coordinates_relative_to_parent(u32 index) -> glm::vec2
{
	return vertices[index].coord.position;
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

auto gui::get_renderer(void) -> renderer_2D *
{
	return renderer;
}

auto gui::get_parent_coord(u32 index) -> glm::vec2
{
	return parent->get_coordinates_relative_to_parent(0);
}

auto gui::convert_to_screen_space(f32 display_w, f32 display_h, gui_vertices_cache const & pixel_vertices) -> gui_vertices_cache
{
	gui_vertices_cache cache = pixel_vertices;

	/* convert to 0.0f -> 1.0f from top left */
	cache.coord.position /= glm::vec2(display_w, display_h);
	cache.size.position /= glm::vec2(display_w, display_h);
	/* convert to 0.0f -> 1.0f from bottom left */
	cache.coord.position.y = 1.0f - cache.coord.position.y;
	/* convert to screen space OpenGL coordinates */
	cache.coord.position = cache.coord.position * 2.0f - glm::vec2(1.0f);
	
	cache.size.position *= 2.0f;
	
	cache.coord.texture_coords.y = 1.0f - cache.coord.texture_coords.y;
	
	return cache;
}

auto gui::bind_renderer(renderer_2D * renderer) -> void
{
	this->renderer = renderer;
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

auto gui::submit_to_renderer(i32 display_w, i32 display_h) -> void
{
	for (u32 i = 0; i < vertices.size(); ++i)
	{
		gui_vertices_cache cache = vertices[i];

		glm::vec2 global_pos = get_coordinates_relative_to_display(i);

		cache.coord.position = global_pos;

		auto real_vertices_cache = convert_to_screen_space(display_w, display_h, cache);
		auto real_vertices_array = convert_cache_to_vertices(real_vertices_cache);
		renderer->submit(real_vertices_array.vertices);
	}
}

auto convert_cache_to_vertices(gui_vertices_cache const & cache) -> gui_vertices
{
	gui_vertices vertices;

	vertices[0].pos = cache.coord.position;
	(vertices[1].pos = cache.coord.position).y -= cache.size.position.y;
	(vertices[2].pos = cache.coord.position).x += cache.size.position.x;
	vertices[3].pos = cache.coord.position + glm::vec2(cache.size.position.x, -cache.size.position.y);

	vertices[0].tex_coord = cache.coord.texture_coords;
	(vertices[1].tex_coord = cache.coord.texture_coords).y -= cache.size.texture_coords.y;
	(vertices[2].tex_coord = cache.coord.texture_coords).x += cache.size.texture_coords.x;
	vertices[3].tex_coord = cache.coord.texture_coords + glm::vec2(cache.size.texture_coords.x, -cache.size.texture_coords.y);

	return vertices;
}