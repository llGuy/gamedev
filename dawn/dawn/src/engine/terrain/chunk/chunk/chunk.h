#ifndef CHUNK_HEADER
#define CHUNK_HEADER
#include "chunk_space_coord.h"
#include "../../../mesh/mesh.h"

namespace dawn { namespace terrain { namespace chunk {
	
			class Chunk
			{
			public:
				Chunk(const ChunkSpaceCoord& coord);
				~Chunk(void) = default;

				void Init(void);
			private:
				ChunkMesh m_mesh;
			};

		}
	}
}

#endif