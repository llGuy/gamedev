#include "shadows.h"
#include "log.h"
#include <glm/gtx/transform.hpp>

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
	depth_texture.bind();
	depth_texture.fill(GL_DEPTH_COMPONENT16, 1024, 1024, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	depth_texture.int_param(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	depth_texture.int_param(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	depth_texture.int_param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	depth_texture.int_param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	depth_texture.int_param(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	depth_texture.int_param(GL_TEXTURE_COMPARE_FUNC, GL_COMPARE_R_TO_TEXTURE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

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
	glm::mat4 depth_projection = glm::ortho<float>(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 30.0f);
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