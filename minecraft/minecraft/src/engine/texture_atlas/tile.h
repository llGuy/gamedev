#ifndef TILE_HEADER
#define TILE_HEADER

#include <glm/glm.hpp>

namespace minecraft
{
	struct TextureAtlasTile
	{
		glm::vec2 tx00;
		glm::vec2 tx01;
		glm::vec2 tx10;
		glm::vec2 tx11;
	};
}

#endif