#pragma once

#include <vector>
#include <unordered_map>

#include "shader.h"
#include "../xcp/exception.h"

class glsl_program
{
private:
	std::vector<glsl_shader> shaders;
	std::unordered_map<std::string, i32> uloc_cache;

	u32 id;
public:
	auto bind(void) -> void;

	auto attach(glsl_shader const & sh) -> void;
public:
	auto send_uniform_vec2(std::string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_vec3(std::string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_vec4(std::string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_mat4(std::string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_float(std::string const & name, float v) -> void;
	auto send_uniform_int(std::string const & name, i32 v) -> void;
	auto send_uniform_intv(std::string const & name, i32 * v, i32 amount) -> void;
public:
	template <typename ... T> auto link(T ... attrib_names) -> void
	{
		id = glCreateProgram();

		attach_shaders();
		bind_attribs(attrib_names...);

		glLinkProgram(id);

		destroy_shaders();

		check_status();
	}
private:
	auto attach_shaders(void) -> void;
	auto destroy_shaders(void) -> void;

	auto check_status(void) -> void;

	auto get_uniform_location(std::string const & name)->i32;
private:
	template <typename ... T> auto bind_attribs(T ... attribs) -> void
	{
		u32 i = 0;

		(glBindAttribLocation(id, i++, attribs), ...);
	}
};