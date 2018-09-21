#pragma once

#include <array>
#include "post_proc_stage.h"
#include "hblur_stage.h"
#include "vblur_stage.h"
#include "default_stage.h"

extern auto render_model(renderable & model, GLenum type) -> void;
extern auto clear(i32 flags, f32 r, f32 g, f32 b) -> void;

struct blur_stage { hblur_stage h; vblur_stage v; };

class render_pipeline
{
public:
	render_pipeline(void);

	auto create(i32 w, i32 h, resource_handler & rh) -> void;
	auto bind_default(void) -> void;
	auto bind_glow(void) -> void;

	auto finalize_process(void) -> void;
private:
	auto carry_blur(void) -> void;
private:
	default_stage default_target;

	std::array<blur_stage, 3> blur_stages;

	gui_quad render_quad;
	program shaders;
};