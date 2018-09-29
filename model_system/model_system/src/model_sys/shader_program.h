#pragma once

#include <array>
#include <vector>
#include <sstream>
#include "shader.h"
#include "xcp.h"

namespace xcp {

	class shader_link_error : public gl_xcp
	{
	private:
		std::vector<std::string> file_names;

		std::string link_error;
	public:
		shader_link_error(std::string const & err, std::vector<shader> const & shaders)
			: link_error(err), gl_xcp::gl_xcp("program link error")
		{
			for (u32 i = 0; i < shaders.size(); ++i)
			{
				file_names.push_back(shaders[i].get_file_name());
			}
		}

		virtual auto what(void) const -> char const *
		{
			std::ostringstream str;

			str << gl_xcp::what() << " : ";
			for (auto file : file_names)
			{
				str << "\t - " << file << '\n';
			}
			str << link_error;

			return str.str().c_str();
		}
	};

}

class program
{
private:
	std::vector<shader> shaders;

	u32 id;
public:
	auto bind(void) -> void;

	auto attach(shader const & sh) -> void;
public:
	auto send_uniform_vec2(char const * name, float * ptr, u32 count) -> void;
	auto send_uniform_vec3(char const * name, float * ptr, u32 count) -> void;
	auto send_uniform_vec4(char const * name, float * ptr, u32 count) -> void;
	auto send_uniform_mat4(char const * name, float * ptr, u32 count) -> void;
	auto send_uniform_float(char const * name, float v) -> void;
	auto send_uniform_int(char const * name, i32 v) -> void;
public:
	template <typename ... T> auto link(T ... attrib_names) -> void
	{
		id = glCreateProgram();

		attach_shaders();
		bind_attribs(attrib_names);

		glLinkProgram(id);

		destroy_shaders();

		check_status();
	}
private:
	auto attach_shaders(void) -> void;
	auto destroy_shaders(void) -> void;

	auto check_status(void) -> void;
private:
	template <typename ... T> auto bind_attribs(T ... attribs) -> void
	{
		u32 i = 0;

		(glBindAttribLocation(id, i++, attribs), ...);
	}

	auto get_uniform(char const * name) -> u32;
};