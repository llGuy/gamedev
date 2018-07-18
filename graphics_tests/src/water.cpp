#include "water.h"

auto water::create(resource_handler & rh) -> void
{
	water_quad.create(rh);

	water_shaders.create_shader(GL_VERTEX_SHADER, "water_vsh.shader");
	water_shaders.create_shader(GL_FRAGMENT_SHADER, "water_fsh.shader");
	water_shaders.link_shaders("vertex_position");
	water_shaders.get_uniform_locations("projection_matrix", "view_matrix");


	create_reflection_fbo(reflection_width, reflection_height);
	create_refraction_fbo(refraction_width, refraction_height);


}

auto water::bind_framebuffer(framebuffer & fbo, int32_t w, int32_t h) -> void
{
	unbind_all_textures();
	fbo.bind();
	glViewport(0, 0, w, h);
}

auto water::prepare(glm::mat4 & proj, glm::mat4 & view) -> void
{
	water_shaders.use();
	water_shaders.uniform_mat4(&proj[0][0], 0);
	water_shaders.uniform_mat4(&view[0][0], 1);
}

auto water::quad(void) -> quad_3D &
{
	return water_quad;
}

auto water::create_reflection_fbo(int32_t w, int32_t h) -> void
{
	water_reflection_fbo.create();
	water_reflection_fbo.bind();

	create_color_texture_attachment(refl_color, water_reflection_fbo, w, h);
	create_depth_buffer_attachment(refl_depth, water_reflection_fbo, w, h);
}

auto water::create_refraction_fbo(int32_t w, int32_t h) -> void
{
	water_refraction_fbo.create();
	water_refraction_fbo.bind();

	create_color_texture_attachment(refr_color, water_refraction_fbo, w, h);
	create_depth_texture_attachment(refr_depth, water_refraction_fbo, w, h);
}

auto water::create_color_texture_attachment(texture & t, framebuffer & fbo, int32_t w, int32_t h) -> void
{
	t.create();
	t.bind();
	t.fill(GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	t.int_param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	t.int_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	fbo.attach(t, GL_COLOR_ATTACHMENT0, 0);
}

// needs to pass a depth_texture type
auto water::create_depth_texture_attachment(depth_texture & t, framebuffer & fbo, int32_t w, int32_t h) -> void
{
	t.set_dimensions(w, h);
	t.create();
	fbo.attach(t, GL_DEPTH_ATTACHMENT, 0);
}

auto water::create_depth_buffer_attachment(renderbuffer & r, framebuffer & fbo, int32_t w, int32_t h) -> void
{
	r.create();
	r.bind();
	r.set_storage(GL_DEPTH_COMPONENT, w, h);
	fbo.attach(r, GL_DEPTH_ATTACHMENT);
}

auto water::bind_refl(void) -> void
{
	bind_framebuffer(water_reflection_fbo, reflection_width, reflection_height);
}

auto water::bind_refr(void) -> void
{
	bind_framebuffer(water_refraction_fbo, refraction_width, refraction_height);
}

auto water::unbind_framebuffers(int32_t w, int32_t h) -> void
{
	unbind_all_framebuffers();
	glViewport(0, 0, w, h);
}

auto water::refl_texture(void) -> texture &
{
	return refl_color;
}

auto water::refr_texture(void) -> texture &
{
	return refr_color;
}