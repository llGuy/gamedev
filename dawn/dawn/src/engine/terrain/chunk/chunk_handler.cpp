#include "chunk_handler.h"

namespace dawn { namespace terrain { namespace chunk {
		
			ChunkHandler::ChunkHandler(int32_t seed, ent::Entity* player)
				: m_loader(seed, &m_chunkMap, player)
			{
			}

		}
	}
}