#pragma once

#include "font.h"
#include "../gui.h"
#include <glm/glm.hpp>

class font_handler;

class font_stream : public gui
{
private:
	u32 font_id;

	f32 font_size;

	std::string text;

	f32 line_height;
public:
	font_stream(u32 font_id, f32 font_size, f32 line_height);

	auto submit_text(std::string const & text) -> void;

	auto update(font_handler & fonts) -> void;

	//auto submit_to_renderer(i32 display_w, i32 display_h) -> void override;
};