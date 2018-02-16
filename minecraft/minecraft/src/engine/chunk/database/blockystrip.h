#ifndef BLOCKYSTRIP
#define BLOCKYSTRIP

#include <unordered_map>
#include "../../block/block.h"
#include "../../terrain/biome/biome.h"

namespace minecraft
{
	namespace chunk
	{
		typedef std::unordered_map<signed int, Block> ysmap_t;

		struct BlockYStrip
		{
			ysmap_t ystrip;
			signed int smallest;
			signed int top;
			biome::biome_t bio;
		};
	}
}

#endif