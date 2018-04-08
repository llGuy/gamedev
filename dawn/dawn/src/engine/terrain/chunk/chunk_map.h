#ifndef CHUNK_MAP
#define CHUNK_MAP
#include <unordered_map>
#include "chunk/chunk.h"
#include "chunk/chunk_space_coord.h"

namespace dawn { namespace terrain { namespace chunk {

			// chunk map that will store all the chunks
			using CMap = std::unordered_map<ChunkSpaceCoord, Chunk>;

		} 
	} 
}

#endif
