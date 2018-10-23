#pragma once

#include "font.h"
#include "../../utils/types.h"

class font_stream;
class renderer_2D;

class font_handler
{
private:
	std::unordered_map<std::string, u32> font_stream_index_map;

	std::unordered_map<std::string, u32> index_map;

	std::vector<font_stream *> streams;

	std::vector<font> fonts;
public:
	auto create_font_stream(std::string const & stream_name, std::string const & name, glm::vec2 const & size, f32 line_height, renderer_2D * renderer) -> u32;

	auto create_font(texture_mapper & textures, std::string const & name, std::string const & dir) -> u32;

	auto get_font_index(std::string const & name) -> u32;

	auto get_font(u32 index) -> font &;

	auto get_font_stream(u32 index) -> font_stream *;

	auto get_font_stream_index(std::string const & name) -> u32;
};
