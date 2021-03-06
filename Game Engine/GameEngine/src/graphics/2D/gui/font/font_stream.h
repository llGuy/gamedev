#pragma once

#include "font.h"
#include "../gui.h"

class font_handler;

class font_stream : public gui
{
private:
	font * bound_font;

	glm::vec2 font_size;

	std::string text;

	f32 line_height;
public:
	font_stream(void) = default;

	auto init(font * bound_font, glm::vec2 const & font_size, f32 line_height) -> void;

	auto update(void) -> void override;

	auto clear(void) -> void;

	template <typename ... T> auto submit_text(T && ... element) -> void
	{
		std::stringstream stream;
		(stream << ... << element);
		text += stream.str();
	}
};