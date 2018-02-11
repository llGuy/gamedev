#include "chunk_loader.h"
#include "../../../log.h"

#include <chrono> 

namespace minecraft
{
	namespace chunk
	{
		namespace loader
		{
			void CLoader::UpdateChunksUnder(std::function<bool(int, int)> xf, std::function<void(int&)> xi,
				std::function<bool(int, int)> zf, std::function<void(int&)> zi,
				int startx, int startz)
			{
				for (;;)
				{
					chunk::Chunk::WCoordChunk wc = PlayerWPosInChunkCoordinates();

					if (wc != m_playerCurrentChunkCoordinates)
					{
						for (int z = wc.wpos.z + startz; zf(z, wc.wpos.z - startz); zi(z))
						{
							for (signed int x = wc.wpos.x + startx; xf(x, wc.wpos.x - startx); xi(x))
							{
								WVec2 c = { x, z };
								chunk::Chunk::WCoordChunk wcc = c;
								for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
								{
								}
								chunk::Chunk& ch = m_currentMap->operator[](wcc);
								if (!(ch.Loaded()) && !ch.BufferLoaded())
								{
									std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
									m_currentMap->operator[](wcc) = Chunk(wcc, m_seed, *m_currentTerrain);
									std::chrono::high_resolution_clock::duration d = std::chrono::high_resolution_clock::now() - tp;
									std::cout << d.count() / 1000000.0 << std::endl;
								}
							}
						}
					}
					m_playerCurrentChunkCoordinates = wc;
				}
			}
			void CLoader::UpdateChunksDistant(void)
			{
				for (;;)
				{
					chunk::Chunk::WCoordChunk wc = PlayerWPosInChunkCoordinates();
					glm::vec3 playerViewDirection = *m_player->EntityViewDirection();
					signed int x = static_cast<signed int>(playerViewDirection.x * 8.0f) + wc.wpos.x;
					signed int z = static_cast<signed int>(playerViewDirection.z * 8.0f) + wc.wpos.z;

					signed int diffx = (x == 0 ? -2 : (abs(x) / x) * 2);
					signed int diffz = (z == 0 ? -2 : (abs(z) / z) * 2);

					x = (x == 0 ? -2 : x + (abs(x) / x) * 2);
					z = (z == 0 ? -2 : z + (abs(z) / z) * 2);

					for (signed int zi = wc.wpos.z - diffz; zi != z + diffz; (zi > z + diffz ? --zi : ++zi))
					{
						for (signed int xi = wc.wpos.x - diffx; xi != x + diffx; (xi > x + diffx ? --xi : ++xi))
						{
							WVec2 c = { xi, zi };
							chunk::Chunk::WCoordChunk wcc = c;
							for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
							{
							}
							chunk::Chunk& ch = m_currentMap->operator[](wcc);
							if (!(ch.Loaded()) && !ch.BufferLoaded())
							{
								std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
								m_currentMap->operator[](wcc) = Chunk(wcc, m_seed, *m_currentTerrain);
								std::chrono::high_resolution_clock::duration d = std::chrono::high_resolution_clock::now() - tp;
								std::cout << d.count() / 1000000.0 << std::endl;
							}
						}
					}
				}
			}
			CLoader::CLoader(cmap::CMap* cm, ent::Entity* player, signed int seed, terrain::Terrain& t)
				: m_currentMap(cm), m_player(player), m_currentTerrain(&t)
			{
			}
			// spawn thread
			void CLoader::Spawn(GLFWwindow* window)
			{
				m_playerCurrentChunkCoordinates = { { -0x551, -0x914} };
				// loads all the chunks under the player
				m_clthread[0] = std::thread(&CLoader::UpdateChunksUnder, this,
					[=](int a, int b) {return a < b; } /* conditional lambda */, [&](int& x) { ++x; },/* incremental lambda */
					[=](int a, int b) {return a < b; } /* conditional lambda */, [&](int& z) { ++z; },/* incremental lambda */ -4, -4);
				// loads the chunks in the direction of the player
				m_clthread[1] = std::thread(&CLoader::UpdateChunksDistant, this);

				m_clthread[0].detach();
				m_clthread[1].detach();
			}
			chunk::Chunk::WCoordChunk CLoader::PlayerWPosInChunkCoordinates(void)
			{
				glm::vec3 v = *m_player->EntityWorldPosition();
				WVec2 xz = { static_cast<signed int>(v.x), static_cast<signed int>(v.z) };
				signed int x = xz.x == 0 ? 0 : (abs(xz.x) + 8) * (xz.x / abs(xz.x)) / 16;
				signed int z = xz.z == 0 ? 0 : (abs(xz.z) + 8) * (xz.z / abs(xz.z)) / 16;
				return { { x, z } };
			}
		}
	}
}