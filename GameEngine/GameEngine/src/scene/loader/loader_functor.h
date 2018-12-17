#pragma once

#pragma once

#include <unordered_map>
#include "../game_object.h"
#include <nlohmann/json.hpp>
#include "../../console/console.h"

#define FUNCTOR_VEC3(attribute) auto apply(scene_loader_functor::data_type & data) -> void override { \
f32 x = data.iterator->value()["x"]; \
f32 y = data.iterator->value()["y"]; \
f32 z = data.iterator->value()["z"]; \
data.obj->get_data().##attribute = glm::vec3(x, y, z); \
}

#define APPLY_FUNC_DATA_TYPE \
struct data_type \
{ \
class model_handler * models;\
\
}

/* functions for initializing entities */
struct scene_loader_functor
{
	struct data_type
	{
		game_object * obj;
		class scene * dest;
		class camera * cam;

		class input_handler * inputs;
		class model_handler * models;
		class material_handler * materials;

		nlohmann::json * handle;
		nlohmann::json::iterator * iterator;
	};

	virtual auto apply(data_type & data) -> void = 0;
};

/* attributes that are stored in the game object data struct */
struct position_functor : scene_loader_functor
{
	FUNCTOR_VEC3(position)
};

struct direction_functor : scene_loader_functor
{
	FUNCTOR_VEC3(direction)
};

struct scale_functor : scene_loader_functor
{
	FUNCTOR_VEC3(scale)
};