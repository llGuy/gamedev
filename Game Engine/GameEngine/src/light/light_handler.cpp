#include "../api/api.h"
#include "light_handler.h"
#include <glm/gtc/type_ptr.hpp>

light_handler::light_handler(void)
	: light{ glm::vec3(10.0f, 10.0f, 10.0f),
	glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f) }
{
}

auto light_handler::prepare_shader(glsl_program & program) -> void
{
	program.send_uniform_vec3("light_info.light_position", glm::value_ptr(light.light_position), 1);
	program.send_uniform_vec3("light_info.ambient_intensity", glm::value_ptr(light.ambient_intensity), 1);
	program.send_uniform_vec3("light_info.diffuse_intensity", glm::value_ptr(light.diffuse_intensity), 1);
	program.send_uniform_vec3("light_info.specular_intensity", glm::value_ptr(light.specular_intensity), 1);
}