#ifndef BIOME_HEADER
#define BIOME_HEADER

namespace minecraft
{
	namespace biome
	{
		enum class biome_t
		{
			DESERT = 0,

			PLAINS = 1,

			MOUNTAINS = 2,

			EXTREME_MOUNTAINS = 3,

			//MEGA_TAIGA = 4,

			OCEAN = 4,

			INV = 2
		};

		// test
		struct Bio
		{
			biome_t b;
			float lerp;	// coefficient that applies to the height
		};
	}
}

#endif