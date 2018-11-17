#include "render_stage2D.h"

render_stage2D::render_stage2D(glsl_program * shader, gui_handler * guis)
	: shader(shader), guis(guis)
{
}

auto render_stage2D::init(u32 w, u32 h) -> void
{
	fbo.create(w, h);
}

auto render_stage2D::execute(void) -> void
{
	glDisable(GL_DEPTH_TEST);

	fbo.bind(GL_FRAMEBUFFER);

	for (u32 i = 0; i < textures2D.size(); ++i)
	{
		textures2D[i]->bind(GL_TEXTURE_2D, i);
	}

	guis->render_one(shader);
}

auto render_stage2D::add_texture2D_bind(texture * tex) -> void
{
	textures2D.push_back(tex);
}