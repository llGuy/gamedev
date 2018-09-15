#pragma once

#include <iostream>
#ifdef cpp_no_optional
#include "optional.hpp"
#else
#include <optional>
#endif
#include <GL/glew.h>
#include "types.h"

class shader
{
public:
	shader(void) = default;
	shader(GLenum type);

	auto compile(std::string const & dir) -> void;
	auto status(void) -> bool;
	auto delete_shader(void) -> void;
	auto id(void)->u32;
	auto type(void)->GLenum;
	auto extract_source(std::string const & dir)->std::optional<std::string>;
private:
	u32 shader_id;
	GLenum shader_type;
};
