#include <array>
#include "../../io/io.h"
#include "environment.h"
#include "../../math/math.h"

auto environment::init_shader(shader_mapper & shaders) -> void
{
	shader_handle sky_shader{ "shader.environment" };
	glsl_shader sky_vs = shaders.create_shader(GL_VERTEX_SHADER, sky_shader, extract_file("src/shaders/environment/environment_vsh.shader"));
	glsl_shader sky_fs = shaders.create_shader(GL_FRAGMENT_SHADER, sky_shader, extract_file("src/shaders/environment/environment_fsh.shader"));
	glsl_program sky_program = shaders.combine(sky_shader, sky_vs, sky_fs);
}

auto environment::init_texture(texture_mapper & textures, std::string const & base_dir) -> void
{
	u32 environment_texture_id = textures.create_texture("texture.environment");
	texture & cubemap_texture = textures[environment_texture_id];

	cubemap_texture.create();
	cubemap_texture.bind(GL_TEXTURE_CUBE_MAP);

	std::array<std::string, 6> files{ "left", "right", "up", "down", "back", "forward" };

	for (u32 i = 0; i < files.size(); ++i)
	{
		auto image = extract_png(base_dir + '/' + files[i] + ".png");
		cubemap_texture.fill(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, image.data, image.w, image.h);
	}

	cubemap_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	cubemap_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	cubemap_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	cubemap_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto environment::calculate_view_matrix(glm::mat4 const & view_matrix) -> glm::mat4
{
	glm::mat4 result(view_matrix);
	result[3][0] = 0.0f;
	result[3][1] = 0.0f;
	result[3][2] = 0.0f;

	return result;
}

auto environment::calculate_translation(void) -> glm::mat4
{
	return glm::scale(glm::vec3(1000.0f));
}

auto environment::submit_to_renderer(renderer_handler & renderers) -> void
{
	auto renderer = renderers.get_renderer_3D(renderer_id);
	renderer->submit(calculate_translation());
}

auto environment::renderer(void) -> u32 &
{
	return renderer_id;
}