#pragma once

#include <vector>
#include "render_stage.h"

#include "uniform_commands.h"

using active_texture_uniform_pair = std::pair<std::string, i32>;

class render_stage2D : public render_stage
{
private:

	glsl_program * shader;
	/* render a quad */
	gui_handler * guis;

	std::vector<texture *> textures2D;
	std::vector<texture *> textures3D;

	std::vector<uniform_command *> extra_commands;

public:

	render_stage2D(glsl_program * shader, gui_handler * guis);

	auto init(u32 w, u32 h) -> void override;

	auto execute(u32 current_id, std::vector<render_stage *> & stages) -> void override;

	template <typename ... T> auto add_texture2D_bind(T * ... textures) -> void
	{
		(textures2D.push_back(textures), ...);
	}

	template <typename ... T> auto add_texture3D_bind(T * ... textures) -> void
	{
		(textures3D.push_back(textures), ...);
	}

	/* T = std::pair<std::string, int> */
	template <typename ... T> auto set_active_textures(T ... pairs) -> void
	{
		shader->bind();
		(shader->send_uniform_int(pairs.first, pairs.second), ...);
	}

	template <typename ... T> auto add_uniform_command(T ... command) -> void
	{
		(([this](uniform_command * com) { com->shader = shader; })(command), ...);

		(extra_commands.push_back(command), ...);
	}

};