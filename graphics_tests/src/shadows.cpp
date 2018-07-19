#include "shadows.h"
#include "log.h"
#include <glm/gtx/transform.hpp>

shadows::shadows(void)
	: depth_texture(1024, 1024), plane(0, -1, 0, 0)
{
}

auto shadows::create(void) -> void
{
	create_fbo();
	create_program();
	create_projection();
}

auto shadows::create_fbo(void) -> void
{
	depth_framebuffer.create();
	depth_framebuffer.bind();

	depth_texture.create();

	depth_framebuffer.attach(depth_texture, GL_DEPTH_ATTACHMENT, 0);

	depth_framebuffer.select_color_buffer(GL_NONE);

	if (depth_framebuffer.framebuffer_status()) logger::sub_log("created shadow fbo");
	else logger::error_log("unable to create shadow fbo");
}

auto shadows::create_program(void) -> void
{
	depth_shader.create_shader(GL_VERTEX_SHADER, "depth_vsh.shader");
	depth_shader.create_shader(GL_FRAGMENT_SHADER, "depth_fsh.shader");
	depth_shader.link_shaders("vertex_position");
	depth_shader.get_uniform_locations("depth_mvp", "model_matrix");
}

auto shadows::create_projection(void) -> void
{
	glm::vec3 light_inv_dir = glm::normalize(glm::vec3(0.2f, 1.0f, 0.2f));

	// compute the MVP matrix from the light's point of view
	glm::mat4 depth_projection = glm::ortho<float>(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 30.0f);
	glm::mat4 depth_view_matrix = glm::lookAt(light_inv_dir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 depth_model_matrix = glm::mat4(1.0f);
	glm::mat4 depth_mvp = depth_projection * depth_view_matrix * depth_model_matrix;

	depth_shader.use();
	depth_shader.uniform_mat4(&depth_mvp[0][0], 0);

	depth_bias = glm::mat4 (

		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f

	);

	depth_bias = depth_bias * depth_mvp;
}

auto shadows::get_inverted_view_matrix(camera & cam) -> glm::mat4
{
	glm::vec3 inverted_position = cam.position();
	glm::vec3 inverted_direction = cam.direction();
	inverted_position.y = -inverted_position.y;
	inverted_direction.y = -inverted_direction.y;

	return glm::lookAt(inverted_position, inverted_position + inverted_direction, glm::vec3(0, 1, 0));
}

auto shadows::shaders(void) -> program &
{
	return depth_shader;
}

auto shadows::fbo(void) -> framebuffer &
{
	return depth_framebuffer;
}

auto shadows::tex(void) -> texture &
{
	return depth_texture;
}

auto shadows::bias(void) -> glm::mat4 &
{
	return depth_bias;
}

auto shadows::depth_projection(void) -> glm::mat4 &
{
	return ortho_projection;
}

auto shadows::clip_plane(void) -> glm::vec4 &
{
	return plane;
}