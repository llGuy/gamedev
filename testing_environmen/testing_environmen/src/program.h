#pragma once


#include <GL/glew.h>
#include <algorithm>
#include <vector>
#include <array>
#include "shader.h"
#include "types.h"

class program
{
public:
	program(void) = default;

	auto use(void) -> void;
	auto create_shader(GLenum type, std::string const & dir) -> void;
public:
	template <typename ... T> auto link_shaders(T ... attribs) -> void
	{
		bool success = true;
		std::for_each(shaders.begin(), shaders.end(),
			[&success](shader & sh) { success &= sh.status(); });
		if (success)
		{
			program_id = glCreateProgram();
			attach_shaders();
			bind_attribs(attribs...);
			glLinkProgram(program_id);
			delete_shaders();
		}
		status();
	}

	template <typename ... T> auto get_uniform_locations(T ... locs) -> void
	{
		std::array<char const *, sizeof...(locs)> names{ locs... };
		std::for_each(names.begin(), names.end(), [&](char const * name)
		{
			u32 location = glGetUniformLocation(program_id, name);
			uniform_locations.push_back(location);
		});
	}
private:
	auto status(void) -> bool;
	auto attach_shaders(void) -> void;
	auto delete_shaders(void) -> void;

	template <typename ... T> auto bind_attribs(T ... attribs) -> void
	{
		std::array<char const *, sizeof...(attribs)> atts{ attribs... };
		for (u32 i = 0; i < sizeof...(attribs); ++i) glBindAttribLocation(program_id, i, atts[i]);
	}
public:
	auto uniform_1f(f32 p, u32 index) -> void;
	auto uniform_2f(f32* p, u32 index) -> void;
	auto uniform_3f(f32* p, u32 index) -> void;
	auto uniform_mat4(f32* p, u32 index) -> void;
	auto uniform_1i(i32 i, u32 index) -> void;
	auto uniform_4f(f32 * p, u32 index) -> void;
private:
	std::vector<shader> shaders;
	std::vector<u32> uniform_locations;
	u32 program_id;
};