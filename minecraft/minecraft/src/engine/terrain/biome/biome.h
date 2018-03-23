#ifndef BIOME_HEADER
#define BIOME_HEADER

namespace minecraft
{
	namespace biome
	{
		enum class biome_t
		{
			ISLANDS,

			OCEAN,

			PLAINS,

			DESERT,

			MOUNTAINS,

			INV
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