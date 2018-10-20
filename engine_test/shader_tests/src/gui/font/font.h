#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include "../../utils/types.h"
#include "../../graphics/texture/texture_mapper.h"

struct character
{
	char character_val;

	glm::vec2 texture_coord;
	glm::vec2 max_texture_coord;

	glm::vec2 offset;
	glm::vec2 size;

	f32 advance_x;
};

class font
{
private:
	u32 texture_id;
	std::string font_name;

	std::unordered_map<char, character> character_info_map;
public:

	font(std::string const & name);

	/* provide just the prefix of the name in the direction e.g. "res/font/consolas" */
	auto load_font(texture_mapper & textures, std::string const & dir) -> void;

private:
	auto load_fnt(std::string const & dir) -> void;
	auto load_image(texture_mapper & textures, std::string const & dir) -> void;
	auto split(std::string const & str, char const splitter)->std::vector<std::string>;
	auto remove_spaces(std::vector<std::string> & words) -> void;
};