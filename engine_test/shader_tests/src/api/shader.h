#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include "../utils/types.h"
#include "../xcp/exception.h"

class glsl_shader
{
private:
	static char const * version;

	u32 id;

	GLenum shader_type;

	std::vector<std::string> srcs;
public:
	glsl_shader(void) = default;

	glsl_shader(GLenum type);

	auto provide(std::string const & src) -> void;

	auto compile(std::string const & dbg_name) -> void;

	auto destroy(void) -> void;

	auto get_id(void) const->u32;
	auto get_file_name(void) const->std::string const &;
private:
	auto extract_source(std::string const &)->std::string;
	auto check_status(std::string const & dbg_name) -> void;
};
