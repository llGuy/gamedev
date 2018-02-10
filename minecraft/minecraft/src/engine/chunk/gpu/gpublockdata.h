#ifndef GPUBLOCKDATA_HEADER
#define GPUBLOCKDATA_HEADER

#include "../../block/block.h"
#include <glm/glm.hpp>

namespace minecraft
{
	namespace chunk
	{
		namespace gpu
		{
			struct BData
			{
				glm::vec3 pos;
				TextureData tex;
			};
		}
	}
}

#endif