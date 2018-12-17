#pragma once

#include <nlohmann/json.hpp>
#include "texture_handler.h"

struct texture_apply_func
{
	virtual auto apply(texture_handler & textures, nlohmann::json::iterator & it, texture & tex) -> void = 0;
};

struct texture_path_apply_func : texture_apply_func
{
	auto apply(texture_handler & textures, nlohmann::json::iterator & it, texture & tex) -> void override
	{
		std::string dir = it.value().find("dir").value();

		bool flip = false;
		if (it.value().find("flip") != it.value().end())
		{
			flip = it.value().find("flip").value();
		}

		bool mipmap = false;
		if (it.value().find("mipmap") != it.value().end())
		{
			mipmap = it.value().find("mipmap").value();
		}

		textures.load_texture_png(dir, &tex, GL_LINEAR, (flip_bool)flip);
		if (mipmap) tex.enable_mipmap(GL_TEXTURE_2D);
	}
};

struct texture_mipmap_apply_func : texture_apply_func
{
	auto apply(texture_handler & textures, nlohmann::json::iterator & it, texture & tex) -> void override
	{
		
	}
};