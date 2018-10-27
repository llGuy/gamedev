#pragma once

#include "font.h"
#include "font_stream.h"
#include "../../renderer/batch_renderer2D.h"

class font_handler
{
private:
	std::unordered_map<std::string, u32> font_stream_index_map;

	std::unordered_map<std::string, u32> index_map;

	std::vector<font_stream *> streams;

	std::vector<font *> fonts;
public:
	auto create_font_stream(std::string const & stream_name, std::string const & name, glm::vec2 const & size, f32 line_height, batch_renderer2D * renderer) -> font_stream *;

	auto create_font(texture_handler & textures, std::string const & name, std::string const & dir) -> font *;

	auto get_font(std::string const & name) -> font *;

	auto get_font_stream(std::string const & name) -> font_stream *;
};
