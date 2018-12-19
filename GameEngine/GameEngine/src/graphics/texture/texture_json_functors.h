#pragma once

#include "texture_handler.h"
#include "json_texture_functor.h"

struct path_texture_functor : texture_loader_functor
{
	auto apply(texture_loader_functor::data_type & data) -> void override
	{
		std::string dir = data.iterator->value().find("dir").value();

		bool flip = false;
		if (data.iterator->value().find("flip") != data.iterator->value().end())
		{
			flip = data.iterator->value().find("flip").value();
		}

		bool mipmap = false;
		if (data.iterator->value().find("mipmap") != data.iterator->value().end())
		{
			mipmap = data.iterator->value().find("mipmap").value();
		}

		data.dest->load_texture_png(dir, data.dest_texture, GL_LINEAR, (flip_bool)flip);
		if (mipmap) data.dest_texture->enable_mipmap(GL_TEXTURE_2D);
	}
};