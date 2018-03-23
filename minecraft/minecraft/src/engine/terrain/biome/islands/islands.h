#ifndef ISLANDS_HEADER
#define ISLANDS_HEADER

#include <stdint.h>

namespace minecraft
{
	namespace biome
	{
		struct IslandsData
		{
			const int32_t OFFSET = 25;
			const int32_t MAX_HEIGHT = 7;
			// start of dirt level
			const float DIRT_LEVEL = 0.95f;
			// start of stone level
			const float STONE_LEVEL = 0.3f;
			// start of bedrock level
			const float BEDROCK_LEVEL = 0.0f;
		};
	}
}

#endif