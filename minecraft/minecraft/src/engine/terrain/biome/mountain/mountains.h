#ifndef MOUNTAINS_HEADER
#define MOUNTAINS_HEADER

namespace minecraft
{
	namespace biome
	{
		struct MountainsData
		{
			const signed int OFFSET = 40;
			const signed int MAX_HEIGHT = 25;
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