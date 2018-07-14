#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#ifdef cpp_no_optional
#include "optional.hpp"
#else
#include <optional>
#endif
#include <GL/glew.h>

class shader
{
public:
	shader(void) = default;
	shader(GLenum type);

	auto compile(std::string const & dir) -> void;
	auto status(void) -> bool;
	auto delete_shader(void) -> void;
	auto id(void) -> uint32_t;
	auto type(void) -> GLenum;
	auto extract_source(std::string const & dir) -> std::optional<std::string>;
private:
	uint32_t shader_id;
	GLenum shader_type;
};

#endif
