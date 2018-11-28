#pragma once

#include "../../api/api.h"
#include "../../api/uniform_buffer.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>

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

struct uniform_mat4 : uniform_command
{
	glm::mat4 value;

	uniform_mat4(std::string const & name, glm::mat4 const & value)
		: uniform_command(name), value(value)
	{
	}
	auto execute(void) -> void override
	{
		shader->send_uniform_mat4(name, glm::value_ptr(value), 1);
	}
};

struct uniform_buffer_bind : uniform_command
{
	uniform_buffer data;

	uniform_buffer_bind(std::string const & name, u32 index)
		: uniform_command(name), data(index)
	{
	}

	auto execute(void) -> void override
	{
		shader->bind_uniform_block(data, name);
	}
};