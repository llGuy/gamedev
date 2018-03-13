#ifndef BIOME_HEADER
#define BIOME_HEADER

namespace minecraft
{
	namespace biome
	{
		enum class biome_t
		{
			PLAINS = 0,

			DESERT = 1,

			MOUNTAINS = 2,

			OCEAN = 3,

			MEGA_TAIGA = 4,

			EXTREME_MOUNTAINS = 5,

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