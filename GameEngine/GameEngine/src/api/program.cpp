#include "program.h"
#include "uniform_buffer.h"

auto glsl_program::attach_shaders(void) -> void
{
	for (auto & shader : shaders)
	{
		glAttachShader(id, shader.get_id());
	}
}

auto glsl_program::destroy_shaders(void) -> void
{
	for (auto & shader : shaders)
	{
		shader.destroy();
	}
}

auto glsl_program::check_status(void) -> void
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

		throw xcp::shader_link_error(result_msg, "");
	}
}

auto glsl_program::bind(void) -> void
{
	glUseProgram(id);
}

auto glsl_program::attach(std::string const & name, glsl_shader const & sh, bool prepend_version) -> void
{
	shaders.push_back(sh);

	shaders.back().compile(name, prepend_version);
}

auto glsl_program::send_uniform_vec2(hashed_string const & name, float * ptr, u32 count) -> void
{
	glUniform2fv(get_uniform_location(name), count, ptr);
}

auto glsl_program::send_uniform_vec3(hashed_string const & name, float * ptr, u32 count) -> void
{
	glUniform3fv(get_uniform_location(name), count, ptr);
}

auto glsl_program::send_uniform_vec4(hashed_string const & name, float * ptr, u32 count) -> void
{
	glUniform4fv(get_uniform_location(name), count, ptr);
}

auto glsl_program::send_uniform_mat4(hashed_string const & name, float * ptr, u32 count) -> void
{
	glUniformMatrix4fv(get_uniform_location(name), count, GL_FALSE, ptr);
}

auto glsl_program::send_uniform_float(hashed_string const & name, float v) -> void
{
	glUniform1f(get_uniform_location(name), v);
}

auto glsl_program::send_uniform_int(hashed_string const & name, i32 v) -> void
{
	glUniform1i(get_uniform_location(name), v);
}

auto glsl_program::bind_uniform_block(uniform_buffer & uniform_block, hashed_string const & name) -> void
{
	auto index = get_uniform_block_index(name);

	uniform_block.bind_base(GL_UNIFORM_BUFFER);

	glUniformBlockBinding(id, index, uniform_block.get_index());
}

auto glsl_program::bind_subroutine(hashed_string const & var, hashed_string const & func_name, GLenum shader) -> void
{
	auto subroutine_index = get_subroutine_index(func_name, shader);
	auto subroutine_location = get_subroutine_location(var, shader);

	//glUniformSubroutinesuiv(shader, 1, &subroutine_index);
}

auto glsl_program::get_subroutine_index(hashed_string const & name, GLenum shader) -> i32
{
	hashed_string_value_type hashed{ name.hashed_value };

	if (auto location = subroutine_cache.find(hashed); location != subroutine_cache.end())
	{
		return location->second;
	}
	return (subroutine_cache[hashed] = glGetSubroutineIndex(id, shader, name.str));
}

auto glsl_program::get_subroutine_location(hashed_string const & name, GLenum shader) -> i32
{
	hashed_string_value_type hashed{ name.hashed_value };

	if (auto location = subroutine_cache.find(hashed); location != subroutine_cache.end())
	{
		return location->second;
	}
	return (subroutine_cache[hashed] = glGetSubroutineUniformLocation(id, shader, name.str));
}

auto glsl_program::get_uniform_location(hashed_string const & name) -> i32
{
	hashed_string_value_type hashed{ name.hashed_value };

	if (auto location = uloc_cache.find(hashed); location != uloc_cache.end())
	{
		return location->second;
	}

	return (uloc_cache[hashed] = glGetUniformLocation(id, name.str));
}

auto glsl_program::get_uniform_block_index(hashed_string const & name) -> i32
{
	hashed_string_value_type hashed{ name.hashed_value };

	if (auto location = block_indices_cache.find(hashed); location != block_indices_cache.end())
	{
		return location->second;
	}

	return (block_indices_cache[hashed] = glGetUniformBlockIndex(id, name.str));
}

auto glsl_program::send_uniform_intv(hashed_string const & name, i32 * v, i32 amount) -> void
{
	glUniform1iv(get_uniform_location(name), amount, v);
}