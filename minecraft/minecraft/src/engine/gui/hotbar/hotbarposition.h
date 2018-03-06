#ifndef HOTBARPOSITION_HEADER
#define HOTBARPOSITION_HEADER

#include <stdint.h>

namespace minecraft
{
	namespace gui
	{
		enum position_t
			: int32_t
		{
			BOTTOM = -1,

			TOP = 1
		};
	}
}

#endif