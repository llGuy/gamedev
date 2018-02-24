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

			MEGA_TAIGA,

			INV
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