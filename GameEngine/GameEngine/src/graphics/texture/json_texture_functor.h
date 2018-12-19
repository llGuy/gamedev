#pragma once

#include "../../api/api.h"
#include <nlohmann/json.hpp>

struct texture_loader_functor
{
	struct data_type
	{
		texture * dest_texture;
		class texture_handler * dest;

		nlohmann::json::iterator * iterator;
		nlohmann::json * handle;
	};

	virtual auto apply(data_type & data) -> void = 0;
};