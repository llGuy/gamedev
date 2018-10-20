#pragma once

#include "font.h"
#include <glm/glm.hpp>

class font_stream
{
private:
	f32 max_length;
	glm::vec3 start_pos;

	std::string text;
public:
	font_stream(f32 line_height, f32 font_size);

	auto submit_text(std::string const & text) -> void;
};