#include "font_handler.h"

auto font_handler::create_font(texture_mapper & textures, std::string const & name, std::string const & dir) -> u32
{
	u32 id = fonts.size();
	font new_font(name);
	new_font.load_font(textures, dir);

	fonts.push_back(new_font);
	index_map[name] = id;

	return id;
}

auto font_handler::get_font_index(std::string const & name)->u32
{
	return index_map[name];
}

auto font_handler::operator[](u32 index)->font &
{
	return fonts[index];
}