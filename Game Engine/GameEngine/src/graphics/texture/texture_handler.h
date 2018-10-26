#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../../api/api.h"

class texture_handler
{
private:
	std::vector<texture *> textures;

	std::unordered_map<std::string, u32> index_map;
public:
	auto init_texture(std::string const & name) -> texture *;

	auto load_3D_texture_png(std::string const & from, texture * to) -> void;

	auto load_texture_png(std::string const & from, texture * to) -> void;

	auto get_texture(std::string const & name) -> texture *;
};