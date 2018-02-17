#ifndef STRUCTURES_HANDLER_HEADER
#define STRUCTURES_HANDLER_HEADER

#include "../structure/structure.h"
#include "../tree/tree.h"

namespace minecraft
{
	namespace structures
	{
		class StructuresHandler
		{
		public:
			explicit StructuresHandler(signed int seed);
		public:
			std::vector<Structure*> StructuresOfChunk(chunk::WCoordChunk wcc, WVec2 negCorner) const;
			CVec2 SCompCoordInChunk(const chunk::WCoordChunk& wcc, WVec2) const;
		private:
			chunk::WCoordChunk ChunkCoordOfSComp(const WVec2 v) const;
			std::vector<Structure*> StructuresOfNeighbouringChunks(chunk::WCoordChunk wcc, 
				chunk::WCoordChunk nwcc, WVec2 negCorner) const;
		private:
			signed int m_seed;
		};
	}
}

#endif