#ifndef EXTRMOUNTAINS_HEADER
#define EXTRMOUNTAINS_HEADER

#include <stdint.h>

namespace minecraft
{
	namespace biome
	{
		struct ExtrMountainsData
		{
			const int32_t OFFSET = 50;
			const int32_t MAX_HEIGHT = 30;
			const float STONE_TOP = 0.9f;
			// start of dirt level
			const float DIRT_LEVEL = 0.8f;
			// start of stone level
			const float STONE_LEVEL = 0.3f;
			// start of bedrock level
			const float BEDROCK_LEVEL = 0.0f;
		};
	}
}

#endif