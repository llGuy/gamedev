#include <array>
#include "shader.h"
#include "xcp_io.h"
#include <fstream>

shader::shader(GLenum type, std::string const & file)
	: file_name(file), shader_type(type)
{
}

auto shader::compile(void) -> void
{
	id = glCreateShader(shader_type);

	std::string src = extract_source(file_name);
	std::array<char const *, 1> srcs { src.c_str() };

	glShaderSource(id, 1, srcs.data(), 0);
	glCompileShader(id);

	check_status(file_name);
}

auto shader::check_status(std::string const & dir) -> void
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

		throw xcp::shader_compile_error(dir, result_msg);
	}
}

auto shader::destroy(void) -> void
{
	glDeleteShader(id);
}

auto shader::get_id(void) const -> u32
{
	return id;
}

auto shader::extract_source(std::string const & dir) -> std::string
{
	std::ifstream file("res/" + dir);

	if (!file.good())
	{
		throw xcp::file_open_error(dir);
	}

	return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

auto shader::get_file_name(void) const -> std::string const &
{
	return file_name;
}