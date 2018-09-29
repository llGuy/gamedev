#pragma once

#include <unordered_map>
#include "component_system.h"

#include "../inc/buffer.h"

struct empty {};

using model_prototype = object<empty>;
using model_instance = u32;


struct comp_vertex_buffer { buffer vb; };
struct comp_normal_buffer { buffer nb; };
struct comp_color_buffer { buffer cb; };
struct comp_texture_buffer { buffer tb; };
struct comp_vao { vertex_layout vao; };
struct comp_texture { texture tex; };


class model_handler
{
private:
	/* models don't need any data */
	component_system<empty> data_system;
	vec_dd<model_prototype> models;
public:
	model_handler(void);

	auto init(void) -> void;

	auto create_model(void) -> model_instance;
};