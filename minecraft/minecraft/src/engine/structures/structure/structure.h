#ifndef STRUCTURE_HEADER
#define STRUCTURE_HEADER

#include <vector>

#include "../../block/block.h"
#include "../../chunk/cdata/cvec2.h"
#include "../../chunk/cdata/wchunkcoord.h"
#include <stdint.h>

namespace minecraft
{
	namespace structures
	{
		enum class structure_t
		{
			TREE
		};

		// from which point to which point
		// of the structure it should be generated
		struct GenRange
		{
			signed int nx;
			signed int px;
			signed int nz;
			signed int pz;
		};

		struct StructComp
		{
			signed int x, z;
			Block::block_t block;
		};

		struct StructCompBYS
		{
			StructComp* bys;
			unsigned int start;
			unsigned int end;
		};

		class Structure
		{
		public:
			explicit Structure(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, 
				GenRange sr, WVec2 worldCoord, structure_t s, const int32_t height);
			explicit Structure(chunk::WCoordChunk wcc, GenRange sr, WVec2 worldCoord, structure_t s, const int32_t height);
		public:
			virtual GenRange& GRange(void) final;
			virtual CVec2 OriginC(void) const final;
			virtual WVec2 OriginW(void) const final;
			virtual int32_t Height(void) const final;
			virtual structure_t SType(void) const final;
			virtual GenRange GenerateStructRangeAll(void) const;
			virtual StructCompBYS GenerateYStripOfStruct(WVec2 worldCoord);
		protected:
			chunk::WCoordChunk m_worldChunkCoord;
			GenRange m_structGenRange;
			CVec2 m_structureOriginChunk;
			const WVec2 m_structureOriginWorld;
			structure_t m_structureType;
			int32_t m_oheight;
		};
	}
}

#endif