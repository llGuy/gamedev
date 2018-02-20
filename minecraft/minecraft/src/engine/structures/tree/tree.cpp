#include "tree.h"

namespace minecraft
{
	namespace structures
	{
		const int32_t Tree::SIZE[tsize_t::INV]
		{
			5,

			6,

			7,

			10
		};

		Tree::Tree(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, GenRange sr, 
			WVec2 wc, structure_t st, const int32_t height, uint32_t size)
			: Structure(wcc, originCoordInChunk, sr, wc, st, height), STRUCT_MAX_HEIGHT(size),
			NX(-2), PX(2), NZ(-2), PZ(2)
		{
		}
		Tree::Tree(chunk::WCoordChunk wcc, GenRange sr, WVec2 worldCoord,
			structure_t s, const int32_t height, uint32_t size)
			: Structure(wcc, sr, worldCoord, s, height), STRUCT_MAX_HEIGHT(size),
			NX(-2), PX(2), NZ(-2), PZ(2)
		{
		}
		const uint32_t* Tree::Size(void)
		{
			return &STRUCT_MAX_HEIGHT;
		}
		GenRange Tree::GenerateStructRangeAll(void) const
		{
			return { NX, PX, NZ, PZ };
		}
		StructCompBYS Tree::GenerateYStripOfStruct(WVec2 worldCoord) 
		{
			int32_t x = worldCoord.x - m_structureOriginWorld.x;
			int32_t z = worldCoord.z - m_structureOriginWorld.z;
			
			if (true) return GenerateYStripOfStructOfOrdinaryTree(x, z);
		}
		StructCompBYS Tree::GenerateYStripOfStructOfOrdinaryTree(int32_t x, int32_t z)
		{
			if (abs(x) == 2.0f || abs(z) == 2.0f)
			{
				for (uint32_t y = STRUCT_MAX_HEIGHT - 3; y <= STRUCT_MAX_HEIGHT - 1; ++y)
					m_utilityArr[y] = { Block::block_t::LEAVES };

				return { m_utilityArr, STRUCT_MAX_HEIGHT - 3, STRUCT_MAX_HEIGHT - 1 };
			}
			else if (abs(x) == 1.0f || abs(z) == 1.0f)
			{
				for (uint32_t y = STRUCT_MAX_HEIGHT - 3; y <= STRUCT_MAX_HEIGHT; ++y)
					m_utilityArr[y] = { Block::block_t::LEAVES };
				return { m_utilityArr, STRUCT_MAX_HEIGHT - 3, STRUCT_MAX_HEIGHT };
			}
			else if (abs(x) == 0.0f && abs(z) == 0.0f)
			{
				for (uint32_t y = 1; y <= STRUCT_MAX_HEIGHT - 1; ++y)
					m_utilityArr[y] = { Block::block_t::OAK_LOG };
				m_utilityArr[STRUCT_MAX_HEIGHT] = { Block::block_t::LEAVES };;
				return { m_utilityArr, 1, STRUCT_MAX_HEIGHT };
			}
			else
				return StructCompBYS();
		}
		StructCompBYS Tree::GenerateYStripOfStructOfGiganticTree(int32_t x, int32_t z)
		{
			return StructCompBYS();
		}
	}
}	