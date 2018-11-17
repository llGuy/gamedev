#pragma once

#include <vector>
#include "render_stage.h"

class render_stage2D : public render_stage
{
private:

	glsl_program * shader;
	/* render a quad */
	gui_handler * guis;

	std::vector<texture *> textures2D;

public:

	render_stage2D(glsl_program * shader, gui_handler * guis);

	auto init(u32 w, u32 h) -> void override;

	auto execute(void) -> void override;

	auto add_texture2D_bind(texture * tex) -> void;

};