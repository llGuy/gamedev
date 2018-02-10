#ifndef PLAINS_HEADER
#define PLAINS_HEADER

namespace minecraft
{
	namespace biome
	{
		struct PlainData
		{
			const signed int OFFSET = 30;
			const signed int MAX_HEIGHT = 10;
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