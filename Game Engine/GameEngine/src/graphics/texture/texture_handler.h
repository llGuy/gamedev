#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../../api/api.h"

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
public:
	auto init_texture(std::string const & name) -> texture *;

	auto load_3D_texture_png(std::string const & from, texture * to) -> void;

	auto load_texture_png(std::string const & from, texture * to, GLenum mag_filter, flip_bool flip = dont_flip_vertically) -> void;

	auto get_texture(std::string const & name) -> texture *;
};