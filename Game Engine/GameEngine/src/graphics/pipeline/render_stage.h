#pragma once

#include "../../api/api.h"
#include "../2D/gui/gui_handler.h"
#include "../texture/texture_handler.h"

class render_stage
{
protected:

	framebuffer fbo;
	/* could be a post processing stage or a 3D scene stage */

	std::vector<texture *> output_textures;

public:

	virtual auto init(u32 w, u32 h) -> void = 0;
	virtual auto execute(u32 current_id, std::vector<render_stage *> & stages) -> void = 0;

	inline
	auto get_fbo(void) -> framebuffer &
	{
		return fbo;
	}

	inline
	auto set_to_default(u32 w, u32 h) -> void
	{
		fbo.width() = w;
		fbo.height() = h;
	}

	inline
	auto bind(void) -> void 
	{
		fbo.bind(GL_FRAMEBUFFER);
	}

	inline
	auto clear(f32 r, f32 g, f32 b, f32 a, u32 flags) -> void
	{
		glClear(flags);
		glClearColor(r, g, b, a);
	}

	inline
	auto attach_texture(texture & tex, u32 attachment, u32 level) -> void
	{
		fbo.bind(GL_FRAMEBUFFER);
		fbo.attach(tex, attachment, level);

		output_textures.push_back(&tex);
	}

	template <typename ... T> 
	auto set_draw_buffers(T ... ts) -> void
	{
		fbo.draw_buffers(ts...);
	}

	inline
	auto attach_renderbuffer(renderbuffer & buff, u32 attachment) -> void
	{
		fbo.bind(GL_FRAMEBUFFER);
		fbo.attach(buff, attachment);
	}

	auto get_output_textures(void) -> std::vector<texture *> &
	{
		return output_textures;
	}

};