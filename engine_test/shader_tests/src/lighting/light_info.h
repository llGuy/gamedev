#pragma once

#include <glm/glm.hpp>

struct light_info
{

	glm::vec3 light_position;

	glm::vec3 ambient_intensity;

	glm::vec3 diffuse_intensity;

	glm::vec3 specular_intensity;

};