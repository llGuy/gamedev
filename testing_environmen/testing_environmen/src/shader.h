#pragma once

#include <string>
#include <sstream>
#include <optional>
#include <stdexcept>
#include <GL/glew.h>
#include "xcp.h"
#include "types.h"


namespace xcp {

	class shader_compile_error : public gl_xcp
	{
	private:
		std::string file_name;
		std::string compile_error;
	public:
		shader_compile_error(std::string const & file, std::string const & err)
			: file_name(file), compile_error(err), gl_xcp::gl_xcp("shader compile error")
		{
		}

		virtual auto what(void) const -> char const * override
		{
			std::ostringstream str;

			str << gl_xcp::what() << " : " << file_name << '\n';
			str << compile_error;

			return str.str().c_str();
		}
	};

}

class shader
{
private:
	u32 id;

	GLenum shader_type;

	std::string file_name;
public:
	shader(void) = default;

	shader(GLenum type, std::string const & file);

	auto compile(void) -> void;

	auto destroy(void) -> void;

	auto get_id(void) const -> u32;
	auto get_file_name(void) const -> std::string const &;
private:
	auto extract_source(std::string const &) -> std::string;
	auto check_status(std::string const & dir) -> void;
};