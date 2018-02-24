#ifndef MEGA_TAIGA_HEADER
#define MEGA_TAIGA_HEADER

#include <stdint.h>
#include <iostream>
#include <ctime>

namespace minecraft
{
	namespace biome
	{
		struct MTaigaData
		{
			const int32_t OFFSET = 35;
			const int32_t MAX_HEIGHT = 13;
			const float DIRT_LEVEL = 0.8f;
			const float STONE_LEVEL = 0.3f;
			const float BEDROCK_LEVEL = 0.0f;
			
			enum
				: bool
			{
				GRASS = true,

				DIRT = false
			};

			bool GrassOfDirt(int32_t seed)
			{
				int32_t random = rand() % 20;
				return false;
			}
		};
	}
}

#endif