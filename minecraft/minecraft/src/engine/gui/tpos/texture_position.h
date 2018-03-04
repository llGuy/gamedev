#ifndef TEXTURE_POSITION_HEADER
#define TEXTURE_POSITION_HEADER

#include <glm/glm.hpp>

namespace minecraft
{
	namespace gui
	{
		struct TPosition
		{
			// position and texture coordinate
			glm::vec2 p, t;
		};
	}
}

#endif