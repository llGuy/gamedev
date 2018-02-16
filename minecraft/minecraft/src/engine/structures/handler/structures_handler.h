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
			std::vector<Structure*> StructuresOfChunk(chunk::WCoordChunk wcc, WVec2 negCorner);
		private:
			signed int m_seed;
		};
	}
}

#endif