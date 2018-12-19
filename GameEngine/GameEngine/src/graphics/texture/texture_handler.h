#pragma once

#include <string>
#include <vector>
#include <unordered_map>
//#include "json_loader.h"
#include "../../api/api.h"
#include "json_texture_functor.h"
#include "../../json_loader.h"

enum flip_bool : bool
{
	dont_flip_vertically = false,

	flip_vertically = true,

	dont_flip_horizontally = false,

	flip_horizontally = true
};

class texture_handler
{
private:
	std::vector<texture *> textures;

	std::unordered_map<std::string, u32> index_map;

	json_loader<texture_loader_functor> loader;
public:
	auto init(void) -> void;

	auto load_from_json(void) -> void;

	auto init_texture(std::string const & name) -> texture *;

	auto load_3D_texture_png(std::string const & from, texture * to) -> void;

	auto load_texture_png(std::string const & from, texture * to, GLenum mag_filter, flip_bool flip = dont_flip_vertically) -> void;

	auto get_texture(std::string const & name) -> texture *;
};