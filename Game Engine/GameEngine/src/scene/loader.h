#pragma once

#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../console/console.h"

#define APPLY_FUNC_VEC3(attribute) 	auto apply(class scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void override { \
f32 x = it.value()["x"]; \
f32 y = it.value()["y"]; \
f32 z = it.value()["z"]; \
obj.get_data().##attribute = glm::vec3(x, y, z); \
}

/* functions for initializing entities */
struct apply_func
{
	virtual auto apply(class scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void = 0;
};

/* attributes that are stored in the game object data struct */
struct position_apply_func : apply_func 
{ 
	APPLY_FUNC_VEC3(position) 
};

struct direction_apply_func : apply_func 
{
	APPLY_FUNC_VEC3(direction) 
};

struct scale_apply_func : apply_func
{
	APPLY_FUNC_VEC3(scale) 
};

class scene_loader
{
private:
	std::unordered_map<std::string, apply_func *> map_funcs;

	nlohmann::json json_handle;
public:

	auto init(class scene & dest, class input_handler & inputs, class model_handler & models, class material_handler & materials) -> void;

	auto load(std::string const & contents, class scene & dest) -> void;

private:

	auto parse_sub(class scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void;

};