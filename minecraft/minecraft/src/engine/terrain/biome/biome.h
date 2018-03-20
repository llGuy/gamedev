#ifndef BIOME_HEADER
#define BIOME_HEADER

namespace minecraft
{
	namespace biome
	{
		enum class biome_t
		{
			OCEAN = 0,

			DESERT = 1,

			PLAINS = 2,

			MOUNTAINS = 3,

			INV = 4
		};

		// test
		struct Bio
		{
			biome_t b;
			float lerp;	// coefficient that applies to the height
		};

		struct BiomeData
		{
			biome_t b;
			float noise;
		};
	}
}

#endif