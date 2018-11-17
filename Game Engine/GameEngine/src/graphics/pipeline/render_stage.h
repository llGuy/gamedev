#pragma once

#include "../../api/api.h"
#include "../2D/gui/gui_handler.h"
#include "../texture/texture_handler.h"

class render_stage
{
protected:

	framebuffer fbo;
	/* could be a post processing stage or a 3D scene stage */

public:

	virtual auto init(u32 w, u32 h) -> void = 0;
	virtual auto execute(void) -> void = 0;

	inline
	auto set_to_default(u32 w, u32 h) -> void
	{
		fbo.width() = w;
		fbo.height() = h;
	}

	inline
	auto clear(f32 r, f32 g, f32 b, f32 a, u32 flags) -> void
	{
		fbo.bind(GL_FRAMEBUFFER);

		glClear(flags);
		glClearColor(r, g, b, a);
	}

	inline
	auto attach_texture(texture & tex, u32 attachment, u32 level) -> void
	{
		fbo.bind(GL_FRAMEBUFFER);
		fbo.attach(tex, attachment, level);
	}

	inline
	auto attach_renderbuffer(renderbuffer & buff, u32 attachment) -> void
	{
		fbo.bind(GL_FRAMEBUFFER);
		fbo.attach(buff, attachment);
	}

};