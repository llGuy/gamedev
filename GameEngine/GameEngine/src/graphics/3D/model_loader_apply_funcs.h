#pragma once

#include "model_handler.h"
#include <nlohmann/json.hpp>

struct model_apply_func
{
	virtual auto apply(model_handler & dest, nlohmann::json::iterator & it, model & renderable) -> void = 0;
};

struct path_apply_func : model_apply_func
{
	auto apply(model_handler & dest, nlohmann::json::iterator & it, model & renderable) -> void override
	{
		std::string format = it.value().find("format").value();
		std::string path = it.value().find("dir").value();

		if (format == ".obj") dest.load_model_from_obj(path, renderable);
		else if (format == ".ubobj") dest.load_model_from_ubobj(path, renderable);
	}
};
