#include "chunk_handler.h"

namespace dawn { namespace terrain { namespace chunk {
		
			ChunkHandler::ChunkHandler(int32_t seed, ent::Entity* player)
				: m_loader(seed, &m_chunkMap, player)
			{
			}

			void ChunkHandler::LoadChunks(int32_t xrange, int32_t zrange)
			{
				for (int32_t z = -(zrange / 2); z <= zrange / 2; ++z)
					for (int32_t x = (-xrange / 2); x <= xrange / 2; ++x)
						m_loader.LoadChunk(ChunkSpaceCoord(x, z));
			}

		}
	}
}