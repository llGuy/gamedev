#pragma once

#include <vector>
#include "texture.h"
#include "renderbuffer.h"
#include "../utils/types.h"

class framebuffer
{
protected:
	u32 id;
	i32 w, h;

	std::vector<texture *> outs;
public:
	framebuffer(void);

	auto create(i32 width, i32 height) -> u32;
	auto clean_up(void) -> void;
	auto bind(GLenum point) -> void;

	template <typename ... T> auto draw_buffers(T ... targets)
	{
		bind(GL_FRAMEBUFFER);
		GLenum buffers[] { static_cast<GLenum>(targets)... };
		glDrawBuffers(sizeof...(T), buffers);
	}

	auto resolve(framebuffer & other) -> void;

	auto add_texture(texture * tx) -> void;
    auto attach(texture & obj, u32 attachment, u32 level) -> void;
	auto attach(renderbuffer & obj, u32 attachment) -> void;

	auto select_color_buffer(u32 point) -> void;
	auto framebuffer_status(void) -> bool;

	auto width(void) -> i32 &;
	auto height(void) -> i32 &;
};

extern auto unbind_all_framebuffers(i32 w, i32 h) -> void;