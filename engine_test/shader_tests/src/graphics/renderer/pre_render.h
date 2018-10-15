#pragma once

#include "../../api/api.h"
#include "../../math/math.h"

struct pre_render
{
	virtual auto prepare(glsl_program & shader) -> void {}
};

struct light_info_pre_render
{
private:
	glm::vec3 light_position;
	glm::vec3 camera_position;
public:
	auto prepare(glsl_program & shader) -> void
	{
		shader.bind();

		shader.send_uniform_vec3("light_position", glm::value_ptr(light_position), 1);
		shader.send_uniform_vec3("camera_position", glm::value_ptr(camera_position), 1);
	}
};

struct material_pre_render : pre_render
{
private:

};