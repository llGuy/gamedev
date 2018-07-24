#ifndef _BLUR_H_
#define _BLUR_H_

#include "program.h"
#include "texture.h"
#include "renderbuffer.h"
#include "framebuffer.h"

class blur_pipeline
{
public:
	auto create(int32_t w, int32_t h) -> void;
	auto prepare_render(void) -> void;

	auto scene_framebuffer(void)->framebuffer &;
	auto hor_framebuffer(void)->framebuffer &;
	auto ver_framebuffer(void)->framebuffer &;

	auto scene_tex(void)->texture &;
	auto horizontal_tex(void) -> texture &;
	auto vertical_tex(void) -> texture &;

	auto prepare_ho_shaders(void) -> void;
	auto prepare_ve_shaders(void) -> void;
private:
	auto create_color_texture_attachment(framebuffer & fbo, texture & tex, int32_t, int32_t) -> void;
	auto create_depth_buffer_attachment(framebuffer & fbo, renderbuffer & rnd, int32_t, int32_t) -> void;
private:
	framebuffer scene_fbo;
	framebuffer bright_fbo;
	framebuffer horizontal_fbo;
	framebuffer vertical_fbo;
	framebuffer combine_fbo;

	texture bright_texture;
	texture scene_texture;
	texture horizontal_texture;
	texture vertical_texture;
	texture combine_texture;

	program bright_program;
	program horizontal_program;
	program vertical_program;
	program combine_program;

	int32_t width, height;
};

#endif