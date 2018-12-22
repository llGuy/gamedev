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
	: light{ glm::vec3(10000000.0f, 10000000.0f, 1000000.0f)
	, glm::vec3(1.0f)
	, glm::vec3(0.8f)
	, glm::vec3(0.8f) }
	, light_uniform_block(LIGHT_BLOCK_INDEX)
	, deferred_block(0)
{
}

auto light_handler::prepare_shader(glsl_program & program) -> void
{
	program.bind_uniform_block(light_uniform_block, "light");
	//program.bind_uniform_block(deferred_block, "light");
	shadows.prepare_shader(program);
}

auto light_handler::update_shadows(f32 far, f32 near, f32 aspect, f32 fov, glm::vec3 const & pos, glm::vec3 const & dir) -> void
{
	shadows.update(far, near, aspect, fov, pos, dir);
	shadows.update_light_view(light.light_position);
}

auto light_handler::create(texture * shadow_map) -> void
{
	shadows.create(light.light_position, shadow_map);

	light_uniform_block.create();

	light_uniform_block.fill<void>(sizeof(light_info_shader_data), nullptr, GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);

	update_buffer();

	deferred_block.create();
	deferred_block.fill<void>(sizeof(int) + sizeof(glm::vec3) * 10, nullptr, GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);
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


	i32 num_lights = 1;
	deferred_block.partial_fill(0, sizeof i32, &num_lights, GL_UNIFORM_BUFFER);
	
	glm::vec3 light_pos1 = light.light_position;
	deferred_block.partial_fill(sizeof i32, sizeof glm::vec3, &light_pos1, GL_UNIFORM_BUFFER);
}