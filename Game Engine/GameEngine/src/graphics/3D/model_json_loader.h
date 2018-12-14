#pragma once

#include "model_handler.h"
#include <nlohmann/json.hpp>
#include "../../animation/animation_handler.h"

struct model_apply_func
{

};

class model_json_loader
{
private:
	std::unordered_map<std::string, model_apply_func *> map_funcs;

	nlohmann::json json_handle;
public:

	auto init(model_handler & models, skeletal_animation_handler & animations) -> void;

	auto load(std::string const & contents, class scene & dest) -> void;

private:

	auto parse_sub(model_handler & models, nlohmann::json::iterator & it, model & renderable) -> void;
};