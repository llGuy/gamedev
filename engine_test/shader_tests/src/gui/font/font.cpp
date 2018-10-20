#include "font.h"
#include <fstream>
#include <algorithm>
#include "../../io/io.h"

font::font(std::string const & name)
	: font_name(name)
{
}

auto font::load_font(texture_mapper & textures, std::string const & dir) -> void
{
	load_fnt(dir);

	load_image(textures, dir);
}

auto font::load_fnt(std::string const & dir) -> void
{
	std::ifstream file(dir + ".fnt");

	std::string line;
	while (std::getline(file, line))
	{
		std::vector<std::string> words = split(line, ' ');


		if (words[0] == "char")
		{
			remove_spaces(words);

			character char_struct;

			/* get character value */
			std::vector<std::string> id = split(words[1], '=');
			char_struct.character_val = static_cast<char>(std::stoi(id[1]));

			/* get character x position */
			std::vector<std::string> posx = split(words[2], '=');
			char_struct.texture_coord.x = std::stof(posx[1]);

			/* get charact y position */
			std::vector<std::string> posy = split(words[3], '=');
			char_struct.texture_coord.y = std::stof(posy[1]);

			/* get character x max position */
			std::vector<std::string> width = split(words[4], '=');
			char_struct.size.x = std::stof(width[1]);

			/* get charact y max position */
			std::vector<std::string> height = split(words[5], '=');
			char_struct.size.y = std::stof(height[1]);

			/* get character x offset */
			std::vector<std::string> xoffset = split(words[6], '=');
			char_struct.offset.x = std::stof(xoffset[1]);

			/* get charact y offset */
			std::vector<std::string> yoffset = split(words[7], '=');
			char_struct.offset.y = std::stof(yoffset[1]);

			/* x advance */
			std::vector<std::string> xadvance = split(words[8], '=');
			char_struct.advance_x = std::stof(xadvance[1]);

			char_struct.max_texture_coord = char_struct.texture_coord + char_struct.size;

			character_info_map[char_struct.character_val] = char_struct;
		}
	}
}

auto font::load_image(texture_mapper & textures, std::string const & dir) -> void
{
	auto image = extract_png(dir + ".png");

	u32 texture_id = textures.create_texture("texture.font." + font_name);
	texture & font_texture = textures[texture_id];
	
	create_color_texture(font_texture, image.w, image.h, image.data);
}

auto font::split(std::string const & str, char const splitter)->std::vector<std::string>
{
	std::vector<std::string> words;
	std::string current;
	std::istringstream iss(str);
	while (std::getline(iss, current, splitter)) words.push_back(current);

	return words;
}

auto font::remove_spaces(std::vector<std::string> & words) -> void
{
	words.erase(std::remove_if(words.begin(), words.end(), 
		[](std::string const & word) { return word == ""; }), words.end());
}