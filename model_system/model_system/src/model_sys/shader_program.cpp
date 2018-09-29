#include "shader_program.h"

auto program::attach_shaders(void) -> void
{
	for (auto & shader : shaders)
	{
		glAttachShader(id, shader.get_id());
	}
}

auto program::destroy_shaders(void) -> void
{
	for (auto & shader : shaders)
	{
		shader.destroy();
	}
}

auto program::check_status(void) -> void
{
	i32 status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		i32 info_log_length = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
		char * buffer = (char *)alloca(info_log_length * sizeof(char));
		i32 buffer_size;
		glGetProgramInfoLog(id, info_log_length * sizeof(char), &buffer_size, buffer);

		std::string result_msg(buffer);

		throw xcp::shader_link_error(result_msg, shaders);
	}
}

auto program::bind(void) -> void
{
	glUseProgram(id);
}

auto program::attach(shader const & sh) -> void
{
	shaders.push_back(sh);

	shaders.back().compile();
}

auto program::send_uniform_vec2(const char * name, float * ptr, u32 count) -> void
{
	glUniform2fv(get_uniform(name), count, ptr);
}

auto program::send_uniform_vec3(const char * name, float * ptr, u32 count) -> void
{
	glUniform3fv(get_uniform(name), count, ptr);
}

auto program::send_uniform_vec4(const char * name, float * ptr, u32 count) -> void
{
	glUniform4fv(get_uniform(name), count, ptr);
}

auto program::send_uniform_mat4(const char * name, float * ptr, u32 count) -> void
{
	glUniformMatrix4fv(get_uniform(name), count, GL_FALSE, ptr);
}

auto program::send_uniform_float(const char * name, float v) -> void
{
	glUniform1f(get_uniform(name), v);
}

auto program::send_uniform_int(const char * name, i32 v) -> void
{
	glUniform1i(get_uniform(name), v);
}

auto program::get_uniform(char const * name) -> u32
{
	return glGetUniformLocation(id, name);
}