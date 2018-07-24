#include "blur.h"

auto blur_pipeline::create(int32_t w, int32_t h) -> void
{
	width = w / 4;
	height = h / 4;

	renderbuffer sc, br, ho, ve, co;

	scene_fbo.create();
	scene_fbo.bind();
	create_color_texture_attachment(scene_fbo, scene_texture, w, h);
	create_depth_buffer_attachment(scene_fbo, sc, w, h);
	scene_fbo.select_color_buffer(GL_COLOR_ATTACHMENT0);

	bright_fbo.create();
	bright_fbo.bind();
	create_color_texture_attachment(bright_fbo, bright_texture, w, h);
	create_depth_buffer_attachment(bright_fbo, br, w, h);
	bright_fbo.select_color_buffer(GL_COLOR_ATTACHMENT0);

	horizontal_fbo.create();
	horizontal_fbo.bind();
	create_color_texture_attachment(horizontal_fbo, horizontal_texture, width, height);
	create_depth_buffer_attachment(horizontal_fbo, ho, width, height);
	horizontal_fbo.select_color_buffer(GL_COLOR_ATTACHMENT0);

	vertical_fbo.create();
	vertical_fbo.bind();
	create_color_texture_attachment(vertical_fbo, vertical_texture, width, height);
	create_depth_buffer_attachment(vertical_fbo, ve, width, height);
	vertical_fbo.select_color_buffer(GL_COLOR_ATTACHMENT0);

	combine_fbo.create();
	combine_fbo.bind();
	create_color_texture_attachment(combine_fbo, combine_texture, width, height);
	create_depth_buffer_attachment(combine_fbo, co, width, height);
	combine_fbo.select_color_buffer(GL_COLOR_ATTACHMENT0);

	horizontal_program.create_shader(GL_VERTEX_SHADER, "horizontal_blur_vsh.shader");
	horizontal_program.create_shader(GL_FRAGMENT_SHADER, "blur_fsh.shader");
	horizontal_program.link_shaders("vertex_position", "texture_coords");
	horizontal_program.get_uniform_locations("target_width");
	horizontal_program.use();
	horizontal_program.uniform_1f(width, 0);

	vertical_program.create_shader(GL_VERTEX_SHADER, "vertical_blur_vsh.shader");
	vertical_program.create_shader(GL_FRAGMENT_SHADER, "blur_fsh.shader");
	vertical_program.link_shaders("vertex_position");
	vertical_program.get_uniform_locations("target_height");
	vertical_program.use();
	vertical_program.uniform_1f(height, 0);
}

auto blur_pipeline::create_color_texture_attachment(framebuffer & fbo, texture & tex, int32_t w, int32_t h) -> void
{
	tex.create();
	tex.bind(GL_TEXTURE_2D);
	tex.fill(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	fbo.attach(tex, GL_COLOR_ATTACHMENT0, 0);
}

auto blur_pipeline::create_depth_buffer_attachment(framebuffer & fbo, renderbuffer & rnd, int32_t w, int32_t h) -> void
{
	rnd.create();
	rnd.bind();
	rnd.set_storage(GL_DEPTH_COMPONENT, w, h);
	fbo.attach(rnd, GL_DEPTH_ATTACHMENT);
}

auto blur_pipeline::prepare_render(void) -> void
{

}

auto blur_pipeline::scene_framebuffer(void) -> framebuffer &
{
	return scene_fbo;
}

auto blur_pipeline::scene_tex(void) -> texture &
{
	return scene_texture;
}

auto blur_pipeline::horizontal_tex(void)->texture &
{
	return horizontal_texture;
}

auto blur_pipeline::vertical_tex(void)->texture &
{
	return vertical_texture;
}

auto blur_pipeline::hor_framebuffer(void)->framebuffer &
{
	return horizontal_fbo;
}

auto blur_pipeline::ver_framebuffer(void)->framebuffer &
{
	return vertical_fbo;
}

auto blur_pipeline::prepare_ho_shaders(void) -> void
{
	horizontal_program.use();
	scene_texture.bind(GL_TEXTURE_2D, 0);
}

auto blur_pipeline::prepare_ve_shaders(void) -> void
{
	vertical_program.use();
	horizontal_texture.bind(GL_TEXTURE_2D, 0);
}