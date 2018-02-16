#include "structures_handler.h"

namespace minecraft
{
	namespace structures
	{
		StructuresHandler::StructuresHandler(signed int seed)
			: m_seed(seed)
		{
		}
		std::vector<Structure*> StructuresHandler::StructuresOfChunk(chunk::WCoordChunk wcc, WVec2 negCorner)
		{
			std::vector<Structure*> v;

			signed int hcomp1 = wcc.wpos.x * 0x145;
			signed int hcomp2 = wcc.wpos.z * 0xf1a;
			signed int hash = std::hash<signed int>()(hcomp1 + hcomp2 + m_seed);
			srand(hash);
			unsigned int size = rand() % 3;
			v.resize(size);
			for (unsigned int i = 0; i < size; ++i)
			{
				unsigned char x = rand() % 16;
				unsigned char z = rand() % 16;

				// for now there are only trees
				v[i] = new Tree(wcc, { x, z }, { -2, 2, -2, 2 }, tsize_t::BIG, {negCorner.x + x, negCorner.z + z});
			}
			return v;
		}
	}
}