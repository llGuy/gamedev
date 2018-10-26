#pragma once

#include <glm/glm.hpp>
#include "../utils/types.h"

struct material_light_info
{
	glm::vec3 ambient_reflectivity;
	glm::vec3 diffuse_reflectivity;
	glm::vec3 specular_reflectivity;
	f32 shininess_factor;
	f32 reflect_factor;

	material_light_info(void) = default;
};

struct light_info
{
	glm::vec3 light_position;
	glm::vec3 ambient_intensity;
	glm::vec3 diffuse_intensity;
	glm::vec3 specular_intensity;

	light_info(void) = default;
};

class light_handler
{
private:

	light_info light;

public:

	light_handler(void);

	auto prepare_shader(glsl_program & program) -> void;

	inline auto light_position(void) -> glm::vec3 &
	{
		return light.light_position;
	}

	inline auto ambient_intensity(void) -> glm::vec3 &
	{
		return light.ambient_intensity;
	}

	inline auto diffuse_intensity(void) -> glm::vec3 &
	{
		return light.diffuse_intensity;
	}

	inline auto specular_intensity(void) -> glm::vec3 &
	{
		return light.specular_intensity;
	}
};