#ifndef CHUNK_HANDLER_HEADER
#define CHUNK_HANDLER_HEADER
#include "chunk/chunk.h"
#include "chunk_map.h"
#include "loader/loader.h"
#include <glm/gtx/hash.hpp>
#include <unordered_map>

namespace dawn { namespace terrain { namespace chunk { 

			class ChunkHandler
			{
			public:
				ChunkHandler(const ChunkHandler&) = delete;
				const ChunkHandler& operator=(const ChunkHandler&) = delete;
				ChunkHandler(int32_t seed, ent::Entity* player);

				void LoadChunks(int32_t xrange, int32_t zrange);

				inline
				auto MapBegin(void)
				{
					return m_chunkMap.begin();
				}
				inline
				auto MapEnd(void)
				{
					return m_chunkMap.end();
				}
			private:
				CMap m_chunkMap;
				ChunkLoader m_loader;
			};

		} 
	} 
}

#endif
