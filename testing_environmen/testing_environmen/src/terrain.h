#pragma once

#include "model_handler.h"
#include "noise_generator.h"

class terrain
{
private:
	i32 width, height;

	model_instance mesh_model_instance;

	program terrain_shaders;
public:
	/* takes the model_instance and adds a color buffer to it */
	auto init(model_instance instance, model_handler & mh, i32 w, i32 d)
	{

	}
};