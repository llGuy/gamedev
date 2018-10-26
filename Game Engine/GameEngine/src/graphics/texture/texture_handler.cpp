#include "../../io/io.h"
#include "texture_handler.h"

auto texture_handler::init_texture(std::string const & texture_name) -> texture * 
{
	textures.push_back(new texture);

	index_map[texture_name] = textures.size() - 1;

	return textures.back();
}

auto texture_handler::load_texture_png(std::string const & from, texture * to) -> void
{
	image png_data = extract_png(from);

	//to->fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, png_data.data, png_data.w, png_data.h);
	create_color_texture(*to, png_data.w, png_data.h, png_data.data, GL_LINEAR);
}