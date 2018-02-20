#ifndef PLAINS_HEADER
#define PLAINS_HEADER

namespace minecraft
{
	namespace biome
	{
		struct PlainData
		{
			const int32_t OFFSET = 25;
			const int32_t MAX_HEIGHT = 7;
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