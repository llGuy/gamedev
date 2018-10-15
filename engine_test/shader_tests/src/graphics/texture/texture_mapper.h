#pragma once

#include "../../api/api.h"
#include <unordered_map>

class texture_mapper
{
private:
	std::vector<texture> textures;
	std::unordered_map<std::string, u32> map_loc;
public:
	texture_mapper(void) = default;

	auto create_texture(std::string const & name) -> u32
	{
		u32 ret = map_loc[name] = textures.size();
		textures.push_back(texture());
		return ret;
	}

	auto get_texture_id(std::string const & name) -> u32
	{
		return map_loc[name];
	}

	auto operator[](u32 index) -> texture &
	{
		return textures[index];
	}
};