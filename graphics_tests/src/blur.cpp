#include "blur.h"

auto blur_pipeline::create(int32_t w, int32_t h) -> void
{
	width = w;
	height = h;

	renderbuffer sc, ho, ve;

	scene_fbo.create();
	scene_fbo.bind();
	create_color_texture_attachment(scene_fbo, scene_texture);
	create_depth_buffer_attachment(scene_fbo, sc);
	scene_fbo.select_color_buffer(GL_COLOR_ATTACHMENT0);
}

auto blur_pipeline::create_color_texture_attachment(framebuffer & fbo, texture & tex) -> void
{
	tex.create();
	tex.bind(GL_TEXTURE_2D);
	tex.fill(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	fbo.attach(tex, GL_COLOR_ATTACHMENT0, 0);
}

auto blur_pipeline::create_depth_buffer_attachment(framebuffer & fbo, renderbuffer & rnd) -> void
{
	rnd.create();
	rnd.bind();
	rnd.set_storage(GL_DEPTH_COMPONENT, width, height);
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