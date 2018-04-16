#ifndef QUAD_HEADER
#define QUAD_HEADER

#include "../tpos/texture_position.h"

namespace minecraft
{
	namespace gui
	{
		struct Quad
		{
			// coordinates
			TPosition cs[4];
		};

		struct QuadV4
		{
			TPositionV4 cs[4];
		};
	}
}

#endif