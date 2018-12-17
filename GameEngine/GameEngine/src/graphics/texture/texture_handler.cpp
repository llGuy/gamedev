#include <array>
#include "../../io/io.h"
#include "texture_handler.h"
#include "../../console/console.h"

auto texture_handler::load_from_json(void) -> void
{
	json_loader.init();

	json_loader.load(extract_file("res/saves/textures.json"), *this);
}

auto texture_handler::init_texture(std::string const & texture_name) -> texture * 
{
	textures.push_back(new texture);
	textures.back()->create();

	index_map[texture_name] = textures.size() - 1;

	return textures.back();
}

auto texture_handler::load_3D_texture_png(std::string const & from, texture * to) -> void
{
	to->create();
	to->bind(GL_TEXTURE_CUBE_MAP);

	std::array<std::string, 6> files{ "left", "right", "up", "down", "back", "forward" };

	for (u32 i = 0; i < files.size(); ++i)
	{
		std::string total_dir = from + '/' + files[i] + ".png";
		auto image = extract_png(total_dir);
		to->fill(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, image.data, image.w, image.h);

		ENG_INFO("Loaded ", total_dir, " (", image.w, "x", image.h, ")");
	}

	to->int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	to->int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	to->int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	to->int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto texture_handler::load_texture_png(std::string const & from, texture * to, GLenum mag_filt , flip_bool flip) -> void
{
	image png_data = extract_png(from, flip);

	create_color_texture(*to, png_data.w, png_data.h, png_data.data, mag_filt);

	ENG_INFO("Loaded ", from, " (", png_data.w, "x", png_data.h, ")");
}

auto texture_handler::get_texture(std::string const & name) -> texture * 
{
	return textures[index_map[name]];
}