#include "apply_funcs.h"
#include "json_loader.h"

auto texture_json_loader::init(void) -> void
{
	map_funcs["load"] = new texture_path_apply_func;
}

auto texture_json_loader::load(std::string const & contents, texture_handler & textures) -> void
{
	json_handle = nlohmann::json::parse(contents);

	for (nlohmann::json::iterator it = json_handle.begin(); it != json_handle.end(); ++it)
	{
		std::string texture_name = "texture." + it.key();

		texture * tex = textures.init_texture(texture_name);

		parse_sub(textures, it, *tex);
	}
}

auto texture_json_loader::parse_sub(texture_handler & textures, nlohmann::json::iterator & it, texture & tex) -> void
{
	for (nlohmann::json::iterator sub_it = it.value().begin(); sub_it != it.value().end(); ++sub_it)
	{
		std::string key = sub_it.key();
		map_funcs[key]->apply(textures, sub_it, tex);
	}
}