#ifndef BLOCKYSTRIP
#define BLOCKYSTRIP

#include <unordered_map>
#include "../../block/block.h"
#include "../../terrain/biome/biome.h"

namespace minecraft
{
	namespace chunk
	{
		typedef std::unordered_map<int32_t, Block> ysmap_t;

		struct BlockYStrip
		{
			ysmap_t ystrip;
			int32_t smallest;
			int32_t top;
			biome::biome_t bio;
		};
	}
}

#endif