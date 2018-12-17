#pragma once

#include "../../api/api.h"
#include <nlohmann/json.hpp>

class texture_json_loader
{
private:
	std::unordered_map<std::string, struct texture_apply_func *> map_funcs;

	nlohmann::json json_handle;
public:
	auto init(void) -> void;

	auto load(std::string const & contents, class texture_handler & textures) -> void;

private:

	auto parse_sub(class texture_handler & textures, nlohmann::json::iterator & it, texture & tex) -> void;
};