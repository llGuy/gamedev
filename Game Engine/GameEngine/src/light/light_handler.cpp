#include "../api/api.h"
#include "light_handler.h"
#include <glm/gtc/type_ptr.hpp>

struct light_info_shader_data
{
	glm::vec4 position;
	glm::vec4 ambient_intensity;
	glm::vec4 diffuse_intensity;
	glm::vec4 specular_intensity;
};

light_handler::light_handler(void)
	: light{ glm::vec3(10.0f, 10.0f, 10.0f)
	, glm::vec3(1.0f)
	, glm::vec3(1.0f)
	, glm::vec3(1.0f) }
	, light_uniform_block(LIGHT_BLOCK_INDEX)
{
}

auto light_handler::prepare_shader(glsl_program & program) -> void
{
	program.bind_uniform_block(light_uniform_block, "light");
}

auto light_handler::create(void) -> void
{
	light_uniform_block.create();

	light_uniform_block.fill<void>(sizeof(light_info_shader_data), nullptr, GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);

	update_buffer();
}

auto light_handler::update_buffer(void) -> void
{
	light_uniform_block.bind(GL_UNIFORM_BUFFER);

	u8 * ptr = (u8 *)(light_uniform_block.map(GL_UNIFORM_BUFFER, GL_READ_ONLY));

	light_info_shader_data data { 
		glm::vec4(light.light_position, 1.0f)
		, glm::vec4(light.ambient_intensity, 1.0f)
		, glm::vec4(light.diffuse_intensity, 1.0f)
		, glm::vec4(light.specular_intensity, 1.0f)};

	memcpy(ptr, &data, sizeof(light_info_shader_data));

	unmap_buffers(GL_UNIFORM_BUFFER);
}