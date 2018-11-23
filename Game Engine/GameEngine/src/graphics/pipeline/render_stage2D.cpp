#include "render_stage2D.h"

render_stage2D::render_stage2D(glsl_program * shader, gui_handler * guis)
	: shader(shader), guis(guis)
{
}

auto render_stage2D::init(u32 w, u32 h) -> void
{
	fbo.create(w, h);
}

auto render_stage2D::execute(u32 current_id, std::vector<render_stage *> & stages) -> void
{
	glDisable(GL_DEPTH_TEST);

	for (u32 i = 0; i < textures2D.size(); ++i)
	{
		if (!textures2D[i])
		{
			auto & prev_outputs = stages[current_id - 1]->get_output_textures();
			u32 set = i;
			do
			{
				prev_outputs[i]->bind(GL_TEXTURE_2D, i++);
			} while (i < prev_outputs.size() + set);
		}
		else
		{
			textures2D[i]->bind(GL_TEXTURE_2D, i);
		}
	}

	if (shader) shader->bind();

	std::for_each(extra_commands.begin(), extra_commands.end()
		, [](uniform_command * command) -> void { 
		command->execute(); 
	});

	guis->render_one(shader);
}