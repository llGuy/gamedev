#include <array>
#include <fstream>
#include "shader.h"

char const * glsl_shader::version = "#version 400\n";

glsl_shader::glsl_shader(GLenum type)
	: shader_type(type)
{
}

auto glsl_shader::provide(std::string const & src) -> void
{
	srcs.push_back(src);
}

auto glsl_shader::compile(std::string const & dbg_name, bool prepend_version) -> void
{
	id = glCreateShader(shader_type);

	std::vector<char const *> srcs_raw;
	
	if (prepend_version)
	{
		srcs_raw.push_back(version);
	}

	for (auto & src : srcs) srcs_raw.push_back(src.c_str());

	glShaderSource(id, srcs_raw.size(), srcs_raw.data(), 0);
	glCompileShader(id);

	check_status(dbg_name);
}

auto glsl_shader::check_status(std::string const & dbg_name) -> void
{
	i32 status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		i32 info_log_length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
		char * buffer = (char *)alloca(info_log_length * sizeof(char));
		i32 buffer_size;
		glGetShaderInfoLog(id, info_log_length * sizeof(char), &buffer_size, buffer);

		std::string result_msg(buffer);

		throw xcp::shader_compile_error(dbg_name, result_msg);
	}
}

auto glsl_shader::destroy(void) -> void
{
	glDeleteShader(id);
}

auto glsl_shader::get_id(void) const -> u32
{
	return id;
}
