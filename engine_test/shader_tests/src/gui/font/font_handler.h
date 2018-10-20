#pragma once

#include "font.h"
#include "../../utils/types.h"

class font_handler
{
private:
	std::unordered_map<std::string, u32> index_map;

	std::vector<font> fonts;
public:
	auto create_font(texture_mapper & textures, std::string const & name, std::string const & dir) -> u32;

	auto get_font_index(std::string const & name) -> u32;

	auto operator[](u32 index) -> font &;
};
