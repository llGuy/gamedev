#ifndef OCEAN_HEADER
#define OCEAN_HEADER

#include <stdint.h>

namespace minecraft
{
	namespace biome
	{
		struct OceanData
		{
			const int32_t OFFSET = -10;
			const int32_t MAX_HEIGHT = 6;
			const float SAND_LEVEL = 0.9f;
			const float DIRT_LEVEL = 0.8f;
			const float STONE_LEVEL = 0.3f;
			const float BEDROCK_LEVEL = 0.0f;
		};
	}
}

#endif