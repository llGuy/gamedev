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

auto program::send_uniform_vec2(std::string const & name, float * ptr, u32 count) -> void
{
	glUniform2fv(get_uniform_location(name), count, ptr);
}

auto program::send_uniform_vec3(std::string const & name, float * ptr, u32 count) -> void
{
	glUniform3fv(get_uniform_location(name), count, ptr);
}

auto program::send_uniform_vec4(std::string const & name, float * ptr, u32 count) -> void
{
	glUniform4fv(get_uniform_location(name), count, ptr);
}

auto program::send_uniform_mat4(std::string const & name, float * ptr, u32 count) -> void
{
	glUniformMatrix4fv(get_uniform_location(name), count, GL_FALSE, ptr);
}

auto program::send_uniform_float(std::string const & name, float v) -> void
{
	glUniform1f(get_uniform_location(name), v);
}

auto program::send_uniform_int(std::string const & name, i32 v) -> void
{
	glUniform1i(get_uniform_location(name), v);
}

auto program::get_uniform_location(std::string const & name) -> i32
{
	if (auto location = uloc_cache.find(name); location != uloc_cache.end())
	{
		return location->second;
	}
	
	return ( uloc_cache[name] = glGetUniformLocation(id, name.c_str()) );
}