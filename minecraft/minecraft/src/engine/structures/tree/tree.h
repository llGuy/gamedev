#ifndef TREE_HEADER
#define TREE_HEADER

#include "../structure/structure.h"

namespace minecraft
{
	namespace structures
	{
		enum tsize_t
		{
			SMALL,

			MEDIUM,

			BIG,

			GIGANTIC,

			INV
		};

		class Tree
			: public Structure
		{
		public:
			static const int32_t SIZE[tsize_t::INV];

			explicit Tree(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, GenRange sr, 
				WVec2 wc, structure_t st, const int32_t height, uint32_t size);
			explicit Tree(chunk::WCoordChunk wcc, GenRange sr, WVec2 worldCoord, 
				structure_t s, const int32_t height, uint32_t size);
			~Tree(void) override
			{
			}
		public:
			const uint32_t* Size(void) override;
			GenRange GenerateStructRangeAll(void) const override;
			StructCompBYS GenerateYStripOfStruct(WVec2 worldCoord) override;
			StructCompBYS GenerateYStripOfStructOfOrdinaryTree(int32_t x, int32_t z);
			StructCompBYS GenerateYStripOfStructOfGiganticTree(int32_t x, int32_t z);
		private:
			const uint32_t STRUCT_MAX_HEIGHT;
			const int32_t NX;
			const int32_t PX;
			const int32_t NZ;
			const int32_t PZ;

			StructComp m_utilityArr[static_cast<uint32_t>(tsize_t::GIGANTIC) + 1];
		};
	}
}

#endif