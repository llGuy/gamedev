#pragma once

#include <vector>
#include "render_stage.h"

struct uniform_command
{
	std::string name;
	glsl_program * shader;

	uniform_command(std::string const & name)
		: name(name)
	{
	}
	virtual auto execute(void) -> void = 0;
};

struct uniform_float : uniform_command
{
	f32 value;

	uniform_float(std::string const & name, f32 value)
		: uniform_command(name), value(value)
	{
	}
	auto execute(void) -> void override
	{
		shader->send_uniform_float(name, value);
	}
};

class render_stage2D : public render_stage
{
private:

	glsl_program * shader;
	/* render a quad */
	gui_handler * guis;

	std::vector<texture *> textures2D;

	std::vector<uniform_command *> extra_commands;

public:

	render_stage2D(glsl_program * shader, gui_handler * guis);

	auto init(u32 w, u32 h) -> void override;

	auto execute(u32 current_id, std::vector<render_stage *> & stages) -> void override;

	auto add_texture2D_bind(texture * tex) -> void;


	inline
	auto add_uniform_command(uniform_command * command) -> void
	{
		command->shader = shader;
		extra_commands.push_back(command);
	}

};