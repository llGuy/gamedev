#ifndef BLOCKYSTRIP
#define BLOCKYSTRIP

#include <unordered_map>
#include "../../block/block.h"

namespace minecraft
{
	namespace chunk
	{
		typedef std::unordered_map<signed int, Block> ysmap_t;

		struct BlockYStrip
		{
			ysmap_t ystrip;
		};
	}
}

#endif