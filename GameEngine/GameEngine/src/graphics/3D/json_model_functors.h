#pragma once

#include "model_handler.h"
#include "json_model_functor.h"

struct path_model_functor : model_loader_functor
{
	auto apply(data_type & data) -> void override
	{
		std::string format = data.iterator->value().find("format").value();
		std::string path = data.iterator->value().find("dir").value();

		if (format == ".obj") data.dest->load_model_from_obj(path, *data.dest_model);
		else if (format == ".ubobj") data.dest->load_model_from_ubobj(path, *data.dest_model);
	}
};