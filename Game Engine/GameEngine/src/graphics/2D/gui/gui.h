#pragma once

#include "../renderer/batch_renderer2D.h"

struct gui_cache
{
	/* from top left */
	vertex2D position;
	vertex2D size;
};

class gui
{
protected:
	texture * diffuse;

	gui * parent;
	std::vector<gui *> children;

	/* gui struct can have multiple quads e.g. fonts */
	std::vector<gui_cache> vertices;

	batch_renderer2D * renderer;
public:
	virtual ~gui(void) = default;

	auto bind_renderer(batch_renderer2D & renderer) -> void;

	auto add_child(gui * child) -> void;

	auto get_coordinates_relative_to_parent(u32 index = 0) -> glm::vec2;

	auto get_coordinates_relative_to_display(u32 index = 0) -> glm::vec2;

	auto get_parent_coord(u32 index = 0)->glm::vec2;

	auto get_renderer(void) -> batch_renderer2D *;

	auto convert_to_screen_space(f32 display_w, f32 display_h, gui_cache const & pixel_vertices) -> gui_cache;

	virtual auto submit_to_renderer(i32 display_w, i32 display_h) -> void;
private:
	auto get_coordinates_relative_to_display_indexed(u32 index = 0)->glm::vec2;
};

extern auto convert_cache_to_vertices(gui_cache const & cache) -> quad2D;