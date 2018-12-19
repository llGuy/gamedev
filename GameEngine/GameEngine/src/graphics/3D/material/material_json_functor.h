#pragma once

#include "material.h"
#include <nlohmann/json.hpp>

struct material_loader_functor
{
	struct data_type
	{
		material_prototype * mat;
		class material_handler * dest;
		class shader_handler * shaders;
		class texture_handler * textures;
		class light_handler * lights;

		nlohmann::json * handle;
		nlohmann::json::iterator * iterator;
	};

	virtual auto apply(data_type & data) -> void = 0;
};