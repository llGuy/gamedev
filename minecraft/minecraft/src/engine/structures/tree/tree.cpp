#include "tree.h"

namespace minecraft
{
	namespace structures
	{
		Tree::Tree(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, GenRange sr, const tsize_t s, 
			WVec2 wc, structure_t st)
			: Structure(wcc, originCoordInChunk, sr, wc, st), STRUCT_MAX_HEIGHT(static_cast<unsigned int>(s)),
			NX(-2), PX(2), NZ(-2), PZ(2)
		{
		}
		Tree::Tree(chunk::WCoordChunk wcc, GenRange sr, WVec2 worldCoord, structure_t s)
			: Structure(wcc, sr, worldCoord, s), STRUCT_MAX_HEIGHT(static_cast<unsigned int>(s)),
			NX(-2), PX(2), NZ(-2), PZ(2)
		{

		}
		GenRange Tree::GenerateStructRangeAll(void) const
		{
			return { NX, PX, NZ, PZ };
		}
		StructCompBYS Tree::GenerateYStripOfStruct(WVec2 worldCoord) 
		{
			signed int x = worldCoord.x - m_structureOriginWorld.x;
			signed int z = worldCoord.z - m_structureOriginWorld.z;
			
			if (abs(x) == 2.0f || abs(z) == 2.0f)
			{
				for (unsigned int y = STRUCT_MAX_HEIGHT - 3; y <= STRUCT_MAX_HEIGHT - 1; ++y)
					m_utilityArr[y] = { x, z,  Block::block_t::LEAVES };
				return {m_utilityArr, STRUCT_MAX_HEIGHT - 3, STRUCT_MAX_HEIGHT - 1};
			}
			else if (abs(x) == 1.0f || abs(z) == 1.0f)
			{
				for (unsigned int y = STRUCT_MAX_HEIGHT - 3; y <= STRUCT_MAX_HEIGHT; ++y)
					m_utilityArr[y] = { x, z,  Block::block_t::LEAVES };
				return { m_utilityArr, STRUCT_MAX_HEIGHT - 3, STRUCT_MAX_HEIGHT };
			}
			else if (abs(x) == 0.0f && abs(z) == 0.0f)
			{
				for (unsigned int y = 1; y <= STRUCT_MAX_HEIGHT - 1; ++y)
					m_utilityArr[y] = { x, z,  Block::block_t::OAK_LOG };
				m_utilityArr[STRUCT_MAX_HEIGHT] = { x, z,  Block::block_t::LEAVES };;
				return { m_utilityArr, 1, STRUCT_MAX_HEIGHT };
			}
			else return StructCompBYS();
		}
	}
}