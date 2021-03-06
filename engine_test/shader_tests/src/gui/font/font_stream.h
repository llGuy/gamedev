#pragma once

#include "font.h"
#include <sstream>
#include "../gui.h"
#include <glm/glm.hpp>

class font_handler;

class font_stream : public gui
{
private:
	u32 font_id;

	glm::vec2 font_size;

	std::string text;

	f32 line_height;
public:

	font_stream(u32 font_id, glm::vec2 const & font_size, f32 line_height);

	auto update(font_handler & fonts) -> void;

	auto clear(void) -> void;

	template <typename ... T> auto submit_text(T && ... element) -> void
	{
		std::stringstream stream;
		(stream << ... << element);
		text += stream.str();
	}

};