#include "loader.h"

namespace dawn { namespace terrain { namespace chunk {
		
			ChunkLoader::ChunkLoader(int32_t seed, CMap* map, ent::Entity* player) noexcept
				: m_chunkMapptr(map), m_playerptr(player), m_seed(seed)
			{
			}

			void ChunkLoader::LoadChunk(const ChunkSpaceCoord& coord)
			{
				m_chunkMapptr->emplace(coord, coord);
			}

		}
	}
}