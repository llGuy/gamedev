#ifndef BIOME_HEADER
#define BIOME_HEADER

namespace minecraft
{
	namespace biome
	{
		enum class biome_t
		{
			PLAINS,

			DESERT,

			MOUNTAINS,

			OCEAN,

			INV
		};

		// test
		struct Biome
		{
			biome_t b;
			float lerp;
		};
	}
}

#endif