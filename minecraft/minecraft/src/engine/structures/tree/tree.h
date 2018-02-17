#ifndef TREE_HEADER
#define TREE_HEADER

#include "../structure/structure.h"

namespace minecraft
{
	namespace structures
	{
		enum class tsize_t
		{
			SMALL = 5,

			MEDIUM = 6,

			BIG = 7
		};

		class Tree
			: public Structure
		{
		public:
			explicit Tree(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, GenRange sr, const tsize_t s, 
				WVec2 wc, structure_t st);
			explicit Tree(chunk::WCoordChunk wcc, GenRange sr, WVec2 worldCoord, structure_t s);
		public:
			GenRange GenerateStructRangeAll(void) const override;
			StructCompBYS GenerateYStripOfStruct(WVec2 worldCoord) override;
		private:
			const unsigned int STRUCT_MAX_HEIGHT;
			const signed int NX;
			const signed int PX;
			const signed int NZ;
			const signed int PZ;

			StructComp m_utilityArr[static_cast<unsigned int>(tsize_t::BIG) + 1];
		};
	}
}

#endif