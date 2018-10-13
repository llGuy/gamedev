#pragma once

#include <sstream>
#include <stdexcept>

namespace xcp {

	class gl_xcp : public std::runtime_error
	{
	public:
		gl_xcp(char const * msg)
			: std::runtime_error::runtime_error(msg)
		{
		}

		virtual auto what(void) const -> char const * override
		{
			return std::runtime_error::what();
		}
	};

	class shader_compile_error : public gl_xcp
	{
	private:
		std::string name;
		std::string compile_error;
	public:
		shader_compile_error(std::string const & name, std::string const & err)
			: name(name), compile_error(err), gl_xcp::gl_xcp("shader compile error")
		{
		}

		virtual auto what(void) const -> char const * override
		{
			static char buffer[10000];

			std::ostringstream str;

			str << gl_xcp::what() << " : " << name << '\n';
			str << compile_error;

			strncpy_s(buffer, str.str().c_str(), 10000);

			return buffer;
		}
	};

	class shader_link_error : public gl_xcp
	{
	private:
		std::string name;

		std::string link_error;
	public:
		shader_link_error(std::string const & err, std::string const & name)
			: link_error(err), gl_xcp::gl_xcp("program link error")
		{
		}

		virtual auto what(void) const -> char const *
		{
			static char buffer[512];

			std::ostringstream str;

			str << gl_xcp::what() << " : " << name << "\n";

			str << link_error;

			strncpy_s(buffer, str.str().c_str(), 512);

			return buffer;
		}
	};

	class file_open_error : public gl_xcp
	{
	private:
		std::string file_name;
	public:
		file_open_error(std::string const & dir)
			: file_name(dir), gl_xcp::gl_xcp("unable to open file")
		{
		}

		virtual auto what(void) const -> char const * override
		{
			static char buffer[512];

			std::ostringstream str;

			str << gl_xcp::what() << " : " << file_name;

			strncpy_s(buffer, str.str().c_str(), 512);

			return buffer;
		}
	};

	class glfw_init_error : public gl_xcp
	{
	private:

	public:
		glfw_init_error(void)
			: gl_xcp::gl_xcp("unable to initialize GLFW")
		{
		}

		virtual auto what(void) const -> char const * override
		{
			return gl_xcp::what();
		}
	};

	class glew_init_error : public gl_xcp
	{
	private:

	public:
		glew_init_error(void)
			: gl_xcp::gl_xcp("unable to initialize GLEW")
		{
		}

		virtual auto what(void) const -> char const * override
		{
			return gl_xcp::what();
		}
	};

	class window_init_error : public gl_xcp
	{
	private:

	public:
		window_init_error(void)
			: gl_xcp::gl_xcp("unable to initialize window")
		{
		}

		virtual auto what(void) const -> char const * override
		{
			return gl_xcp::what();
		}
	};

}