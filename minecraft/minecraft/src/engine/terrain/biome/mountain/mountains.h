#ifndef MOUNTAINS_HEADER
#define MOUNTAINS_HEADER

namespace minecraft
{
	namespace biome
	{
		struct MountainsData
		{
			const int32_t OFFSET = 40;
			const int32_t MAX_HEIGHT = 25;
			const float STONE_TOP = 0.9f;
			// start of dirt level
			const float DIRT_LEVEL = 0.975f;
			// start of stone level
			const float STONE_LEVEL = 0.2f;
			// start of bedrock level
			const float BEDROCK_LEVEL = 0.0f;
		};
	}
}

#endif