#pragma once

#include "gui.h"
#include "panel/panel.h"
#include "font/font_handler.h"
#include "../../shader/shader_handler.h"

#include "../renderer/batch_renderer2D.h"

enum should_render_gui
{
	dont_render_gui = false,

	render_gui = true
};

class gui_handler
{
private:
	std::unordered_map<std::string, u32> index_map;

	std::vector<gui *> gui_sets;

	batch_renderer2D renderer;

	font_handler fonts;
public:
	gui_handler(void) = default;

	/* initializes the shader and renderer */
	auto init(shader_handler & shaders) -> void;

	auto init_font_type(texture_handler & textures, std::string const & font_name, std::string const & dir) -> void;
	auto init_font_stream(std::string const & parent_name, std::string const & stream_name, std::string const & font_name, glm::vec2 const & size, f32 line_height) -> font_stream *;

	/* updates and submits to renderer */
	auto update(u32 display_w, u32 dislay_h) -> void;
	auto render_one(glsl_program * shader) -> void;
	auto render(void) -> void;

	/* gui set = hierarchies of guis */
	template <typename T> auto init_gui_set(std::string const & name, should_render_gui should_render) -> T *
	{
		index_map[name] = gui_sets.size();

		gui_sets.push_back(new T);

		if (should_render)
		{
			gui_sets.back()->bind_renderer(renderer);
		}

		return static_cast<T *>(gui_sets.back());
	}

	template <typename T> auto init_gui(std::string const & parent_name) -> T *
	{
		gui * parent = gui_sets[index_map[parent_name]];

		T * new_gui = new T;

		parent->add_child(new_gui);

		return new_gui;
	}
};