#pragma once

#include "model_data.h"
#include "../../json_loader.h"
#include "../../data/components.h"

struct model_loader_functor
{
	struct data_type
	{
		object<model_data> * dest_model;
		class model_handler * dest;

		nlohmann::json * handle;
		nlohmann::json::iterator * iterator;
	};

	virtual auto apply(data_type & data) -> void = 0;
};

