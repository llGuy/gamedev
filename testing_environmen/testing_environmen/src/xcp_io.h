#pragma once

#include "xcp.h"
#include <sstream>

namespace xcp {

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
			std::ostringstream str;

			str << gl_xcp::what() << " : " << file_name;

			return str.str().c_str();
		}
	};

}