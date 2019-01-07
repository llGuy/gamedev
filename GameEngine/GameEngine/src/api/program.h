#pragma once

#include <vector>
#include <unordered_map>

#include "shader.h"
#include "../utils/detail.h"
#include "../xcp/exception.h"

class uniform_buffer;

class glsl_program
{
private:
	std::vector<glsl_shader> shaders;
	std::unordered_map<hashed_string_value_type, i32> uloc_cache;

	std::unordered_map<hashed_string_value_type, i32> block_indices_cache;
	std::unordered_map<hashed_string_value_type, i32> subroutine_cache;

	u32 id;
public:
	auto bind(void) -> void;

	auto attach(std::string const & name, glsl_shader const & sh, bool prepend_version = true) -> void;
public:
	auto bind_subroutine(hashed_string const & var, hashed_string const & func_name, GLenum shader) -> void;
	auto bind_uniform_block(uniform_buffer & uniform_block, hashed_string const & name) -> void;

	auto send_uniform_vec2(hashed_string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_vec3(hashed_string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_vec4(hashed_string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_mat4(hashed_string const & name, float * ptr, u32 count) -> void;
	auto send_uniform_float(hashed_string const & name, float v) -> void;
	auto send_uniform_int(hashed_string const & name, i32 v) -> void;
	auto send_uniform_intv(hashed_string const & name, i32 * v, i32 amount) -> void;
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

	auto get_subroutine_index(hashed_string const & name, GLenum shader) -> i32;
	auto get_subroutine_location(hashed_string const & name, GLenum shader) -> i32;
	auto get_uniform_location(hashed_string const & name) -> i32;
	auto get_uniform_block_index(hashed_string const & name) -> i32;
private:
	template <typename ... T> auto bind_attribs(T ... attribs) -> void
	{
		u32 i = 0;

		(glBindAttribLocation(id, i++, attribs), ...);
	}
};