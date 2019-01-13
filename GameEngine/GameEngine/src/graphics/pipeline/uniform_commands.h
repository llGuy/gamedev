#pragma once

#include "../../api/api.h"
#include "../../api/uniform_buffer.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>

template<typename V_Type> struct value_holder
{
	using type = V_Type;

	V_Type value;

	value_holder(V_Type const & v)
		: value(v)
	{
	}
};

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

template <typename V_Holder> struct uniform_vec3 : uniform_command
{
	V_Holder * holder;

	uniform_vec3(std::string const & name, V_Holder * value)
		: uniform_command(name), holder(value)
	{
	}
	auto execute(void) -> void override
	{
		shader->send_uniform_vec3(create_hashed_string(name), glm::value_ptr(holder->value), 1);
	}
};

template <typename V_Holder> struct uniform_vec2 : uniform_command
{
	V_Holder * holder;

	uniform_vec2(std::string const & name, V_Holder * value)
		: uniform_command(name), holder(value)
	{
	}
	auto execute(void) -> void override
	{
		shader->send_uniform_vec2(create_hashed_string(name), glm::value_ptr(holder->value), 1);
	}
};

template <typename V_Holder> struct uniform_int : uniform_command
{
	V_Holder * holder;

	uniform_int(std::string const & name, V_Holder * value)
		: uniform_command(name), holder(value)
	{
	}
	auto execute(void) -> void override
	{
		shader->send_uniform_int(create_hashed_string(name), holder->value);
	}
};

template <typename V_Holder> struct uniform_float : uniform_command
{
	V_Holder * holder;

	uniform_float(std::string const & name, V_Holder * value)
		: uniform_command(name), holder(value)
	{
	}
	auto execute(void) -> void override
	{
		shader->send_uniform_float(create_hashed_string(name), holder->value);
	}
};

template <typename V_Holder> struct uniform_mat4 : uniform_command
{
	V_Holder * holder;

	uniform_mat4(std::string const & name, V_Holder * value)
		: uniform_command(name), holder(value)
	{
	}
	auto execute(void) -> void override
	{
		shader->send_uniform_mat4(create_hashed_string(name), glm::value_ptr(holder->value), 1);
	}
};

struct uniform_buffer_bind : uniform_command
{
	uniform_buffer * data;

	uniform_buffer_bind(std::string const & name, uniform_buffer * ubuffer)
		: uniform_command(name), data(ubuffer)
	{
	}

	auto execute(void) -> void override
	{
		shader->bind_uniform_block(*data, create_hashed_string(name));
	}
};