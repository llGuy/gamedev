#pragma once

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

}