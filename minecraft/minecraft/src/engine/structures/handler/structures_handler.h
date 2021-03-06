#ifndef STRUCTURES_HANDLER_HEADER
#define STRUCTURES_HANDLER_HEADER

#include "../structure/structure.h"
#include "../tree/tree.h"
#include "../../terrain/terrain.h"

namespace minecraft
{
	namespace structures
	{
		class StructuresHandler
		{
		public:
			explicit StructuresHandler(int32_t seed);
		public:
			std::vector<Structure*> AllStructuresOfChunk(chunk::WCoordChunk wcc,
				WVec2 negCorner, terrain::Terrain& t);
			std::vector<Structure*> StructuresOfChunk(chunk::WCoordChunk wcc, 
				WVec2 negCorner, terrain::Terrain& t) const;
			CVec2 SCompCoordInChunk(const chunk::WCoordChunk& wcc, WVec2) const;
		private:
			chunk::WCoordChunk ChunkCoordOfSComp(const WVec2 v) const;
			std::vector<Structure*> StructuresOfNeighbouringChunks(chunk::WCoordChunk wcc, 
				chunk::WCoordChunk nwcc, WVec2 negCorner, terrain::Terrain& t) const;
		private:
			int32_t m_seed;
		};
	}
}

#endif