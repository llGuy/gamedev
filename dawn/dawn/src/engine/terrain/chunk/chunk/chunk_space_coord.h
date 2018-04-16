#ifndef CHUNK_SPACE_COORD_HEADER
#define CHUNK_SPACE_COORD_HEADER
#include <glm/glm.hpp>

namespace dawn { namespace terrain { namespace chunk {

			/*	the chunks will be stored in a chunkmap (unordered_map)
				in chunk space coordinates : e.g. chunk at (0, 1) or (1, 2) etc...	
			*/
			using ChunkSpaceCoord = glm::ivec2;

		}
	}
}
#endif