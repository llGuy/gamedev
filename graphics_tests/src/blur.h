#ifndef _BLUR_H_
#define _BLUR_H_

#include "texture.h"
#include "renderbuffer.h"
#include "framebuffer.h"

class blur_pipeline
{
public:
	auto create(int32_t w, int32_t h) -> void;
	auto prepare_render(void) -> void;

	auto scene_framebuffer(void)->framebuffer &;
	auto scene_tex(void)->texture &;
private:
	auto create_color_texture_attachment(framebuffer & fbo, texture & tex) -> void;
	auto create_depth_buffer_attachment(framebuffer & fbo, renderbuffer & rnd) -> void;
private:
	framebuffer scene_fbo;
	framebuffer horizontal_fbo;
	framebuffer vertical_fbo;

	texture scene_texture;
	texture horizontal_texture;
	texture vertical_texture;

	int32_t width, height;
};

#endif