#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "../utils/types.h"

#include "../graphics/renderer/2D/renderer_2D.h"

/* pixel size relative to parent */
struct gui_vertex
{
	glm::vec2 position;
	glm::vec2 texture_coords;
};

struct gui_vertices_cache
{
	gui_vertex coord;
	gui_vertex size;
};

struct gui_vertices
{
	std::array<vertex2D, 4> vertices;

	auto operator[](u32 index) -> vertex2D &
	{
		return vertices[index];
	}
};

/* from top left of screen */
class gui
{
protected:

	gui * parent;
	std::vector<gui *> children;

	std::vector<gui_vertices_cache> vertices;

	renderer_2D * renderer;

public:

	virtual ~gui(void) = default;

	auto bind_renderer(renderer_2D * renderer) -> void;

	auto add_child(gui * child) -> void;

	auto get_coordinates_relative_to_parent(u32 index = 0) -> glm::vec2;

	auto get_coordinates_relative_to_display(u32 index = 0)->glm::vec2;

	auto get_coordinates_relative_to_display_indexed(u32 index = 0) -> glm::vec2;

	auto get_parent_coord(u32 index = 0) -> glm::vec2;

	auto convert_to_screen_space(f32 display_w, f32 display_h, gui_vertices_cache const & pixel_vertices) -> gui_vertices_cache;

	virtual auto submit_to_renderer(i32 display_w, i32 display_h) -> void;

};

extern auto convert_cache_to_vertices(gui_vertices_cache const & cache) -> gui_vertices;