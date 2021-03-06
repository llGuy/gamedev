#pragma once

#include <iostream>
#include <GL/glew.h>
#include "../utils/types.h"

class texture
{
protected:
	u32 id;

	i32 w, h;
public:
	friend class framebuffer;

	virtual auto create(void) -> void;
	auto clean_up(void) -> void;

	auto bind(GLenum target, u32 slot = -1) -> void;
	auto fill(GLenum target, GLenum internal_format,
		GLenum format, GLenum type, void const * data, i32 w, i32 h) -> void;

	auto enable_mipmap(GLenum target) -> void;
	auto float_param(GLenum target, GLenum mode, f32 factor) -> void;
	auto int_param(GLenum target, GLenum mode, GLenum factor) -> void;

	auto get_width(void) -> i32;
	auto get_height(void) -> i32;
	auto get_id(void) -> u32;
};

extern auto unbind_all_textures(GLenum target) -> void;
extern auto create_color_texture(texture &, int32_t w, int32_t h, void * data, GLenum magminfilter, GLenum internal_type = GL_UNSIGNED_BYTE) -> void;
extern auto create_depth_texture(texture & t, int32_t w, int32_t h) -> void;
extern auto create_color_texture_info(texture & t, int32_t w, int32_t h, void * data, GLenum magmin_filter) -> void;