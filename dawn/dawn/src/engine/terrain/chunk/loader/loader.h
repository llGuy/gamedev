#ifndef CHUNK_LOADER_HEADER
#define CHUNK_LOADER_HEADER
#include "../chunk/chunk.h"
#include "../chunk_map.h"
#include "../../../entities/entity.h"

#include <glm/gtx/hash.hpp>

namespace dawn { namespace terrain { namespace chunk {

			class ChunkLoader
			{
			public:
				const ChunkLoader& operator=(const ChunkLoader&) = delete;
				ChunkLoader(const ChunkLoader&) = delete;
				ChunkLoader(int32_t seed, CMap* map, ent::Entity* player) noexcept;

			public:
				void LoadChunk(const ChunkSpaceCoord&);
			private:
				CMap* m_chunkMapptr;
				ent::Entity* m_playerptr;
				int32_t m_seed;
			};
			
		} 
	} 
}

#endif