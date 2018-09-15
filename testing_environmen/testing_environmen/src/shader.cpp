#include "shader.h"
#include <array>
#include <string>
#include <fstream>
#include "log.h"

shader::shader(GLenum type)
	: shader_type(type)
{
}
auto shader::compile(std::string const & dir) -> void
{
	shader_id = glCreateShader(shader_type);
	auto src = extract_source(dir);
	if (src.has_value())
	{
		std::array<char const *, 1> srcs{ src.value().c_str() };
		glShaderSource(shader_id, 1, srcs.data(), 0);
		glCompileShader(shader_id);
	}
}
auto shader::status(void) -> bool
{
	int32_t status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		logger::error_log("failde to create shader");
		int32_t info_log_length = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		char * buffer = (char *)alloca(info_log_length * sizeof(char));
		int32_t buffer_size;
		glGetShaderInfoLog(shader_id, info_log_length * sizeof(char), &buffer_size, buffer);

		logger::new_log("shader compile error info");
		std::cout << buffer << std::endl;
		return false;
	}
	return true;
}
auto shader::delete_shader(void) -> void
{
	glDeleteShader(shader_id);
}
auto shader::id(void) -> u32
{
	return shader_id;
}
auto shader::type(void) -> GLenum
{
	return shader_type;
}
auto shader::extract_source(std::string const & dir)->std::optional<std::string>
{
	std::ifstream file(dir);
	if (!file.good())
	{
		std::cerr << "ERROR : unable to extract from : " << dir << std::endl;
		return std::optional<std::string>{};
	}
	else return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}