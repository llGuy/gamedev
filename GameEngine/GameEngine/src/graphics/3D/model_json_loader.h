#pragma once

#include <nlohmann/json.hpp>
#include "model_data.h"

class model_json_loader
{
private:
	std::unordered_map<std::string, struct model_apply_func *> map_funcs;

	nlohmann::json json_handle;
public:

	auto init(class model_handler & models, class skeletal_animation_handler & animations) -> void;

	auto load(std::string const & contents, class model_handler & dest) -> void;

private:

	auto parse_sub(class model_handler & models, nlohmann::json::iterator & it, object<model_data> & renderable) -> void;
};