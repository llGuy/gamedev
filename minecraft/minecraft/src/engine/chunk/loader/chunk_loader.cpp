#include "chunk_loader.h"
#include "../../../log.h"
#include "../map/mutex/render_update_mutex.h"

#include <chrono> 

namespace minecraft
{
	namespace chunk
	{
		namespace loader
		{
			void CLoader::UpdateChunksUnder(std::function<bool(int32_t, int32_t)> xf, std::function<void(int32_t&)> xi,
				std::function<bool(int32_t, int32_t)> zf, std::function<void(int32_t&)> zi,
				int32_t startx, int32_t startz)
			{
				for (;;)
				{
					if (m_ended) break;
					chunk::Chunk::WCoordChunk wc = PlayerWPosInChunkCoordinates();

					if (wc != m_playerCurrentChunkCoordinates)
					{
						for (int32_t z = wc.wpos.z + startz; zf(z, wc.wpos.z - startz); zi(z))
						{
							for (int32_t x = wc.wpos.x + startx; xf(x, wc.wpos.x - startx); xi(x))
							{
								WVec2 c = { x, z };
								chunk::Chunk::WCoordChunk wcc = c;
								for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
								{
								}
								bool newC = false;
								//chunk::Chunk& ch = m_currentMap->operator[](wcc);
								chunk::Chunk& ch = m_currentMap->At(wcc, &newC);
								if (newC) std::lock_guard<std::mutex> guard(cmap::rumutex);
								if (!(ch.Loaded()) && !ch.BufferLoaded())
								{
									std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
									//ch = Chunk(wcc, m_seed);
									ch.LoadTop(*m_currentTerrain, *m_currentStructuresHandler);
									ch.LoadStructures(*m_currentTerrain, *m_currentStructuresHandler);
									std::chrono::high_resolution_clock::duration d = std::chrono::high_resolution_clock::now() - tp;
								}
							}
						}
					}
					m_playerCurrentChunkCoordinates = wc;
					//m_beginDestroyChunkThread = true;
					chunk::Chunk::WCoordChunk wcc = PlayerWPosInChunkCoordinates();
					glm::vec2 vec2CastWCPlayerPos = glm::vec2(static_cast<float>(wcc.wpos.x), static_cast<float>(wcc.wpos.z));

					std::lock_guard<std::mutex> guard(cmap::dumutex);
					for (auto list = m_currentMap->Begin(); list != m_currentMap->End(); ++list)
					{
						for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
						{
						}

						for (auto c = list->begin(); c != list->end();)
						{
							for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
							{
							}

							chunk::Chunk::WCoordChunk chunkWC = c->ChunkCoordinate();
							glm::vec2 vec2CastCWCChuPos = glm::vec2(static_cast<float>(chunkWC.wpos.x), static_cast<float>(chunkWC.wpos.z));

							if (glm::abs(glm::distance(vec2CastWCPlayerPos, vec2CastCWCChuPos)) > 13.0f)
							{
								std::lock_guard<std::mutex> guard(cmap::rumutex);
								//std::cout << glm::distance(vec2CastWCPlayerPos, vec2CastCWCChuPos) << std::endl;
								c->DestroyGPUBuffer();
								list->erase(c++);
							}
							else ++c;
						}
					}
				}
			}
			void CLoader::UpdateChunksDistant(void)
			{
				for (;;)
				{
					if (m_ended) break;
					chunk::Chunk::WCoordChunk wc = PlayerWPosInChunkCoordinates();
					glm::vec3 playerViewDirection = *m_player->EntityViewDirection();
					int32_t x = static_cast<int32_t>(playerViewDirection.x * 8.0f) + wc.wpos.x;
					int32_t z = static_cast<int32_t>(playerViewDirection.z * 8.0f) + wc.wpos.z;

					int32_t diffx = (x == 0 ? -5 : (abs(x) / x) * 5);
					int32_t diffz = (z == 0 ? -5 : (abs(z) / z) * 5);

					x = (x == 0 ? -5 : x + (abs(x) / x) * 5);
					z = (z == 0 ? -5 : z + (abs(z) / z) * 5);

					for (int32_t zi = wc.wpos.z - diffz; zi != z + diffz; (zi > z + diffz ? --zi : ++zi))
					{
						for (int32_t xi = wc.wpos.x - diffx; xi != x + diffx; (xi > x + diffx ? --xi : ++xi))
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

								ch.LoadTop(*m_currentTerrain, *m_currentStructuresHandler);
								ch.LoadStructures(*m_currentTerrain, *m_currentStructuresHandler);
								//ch = Chunk(wcc, m_seed, *m_currentTerrain, *m_currentStructuresHandler);
								
								std::chrono::high_resolution_clock::duration d = std::chrono::high_resolution_clock::now() - tp;
								//std::cout << d.count() / 1000000.0 << std::endl << std::endl;
							}
						}
					}
					//m_beginDestroyChunkThread = true;
					chunk::Chunk::WCoordChunk wcc = PlayerWPosInChunkCoordinates();
					glm::vec2 vec2CastWCPlayerPos = glm::vec2(static_cast<float>(wcc.wpos.x), static_cast<float>(wcc.wpos.z));

					std::lock_guard<std::mutex> guard(cmap::dumutex);
					for (auto list = m_currentMap->Begin(); list != m_currentMap->End(); ++list)
					{
						for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
						{
						}

						for (auto c = list->begin(); c != list->end();)
						{
							for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
							{
							}

							chunk::Chunk::WCoordChunk chunkWC = c->ChunkCoordinate();
							glm::vec2 vec2CastCWCChuPos = glm::vec2(static_cast<float>(chunkWC.wpos.x), static_cast<float>(chunkWC.wpos.z));

							if (glm::abs(glm::distance(vec2CastWCPlayerPos, vec2CastCWCChuPos)) > 18.0f)
							{
								std::lock_guard<std::mutex> guard(cmap::rumutex);
								//std::cout << glm::distance(vec2CastWCPlayerPos, vec2CastCWCChuPos) << std::endl;
								c->DestroyGPUBuffer();
								list->erase(c++);
							}
							else ++c;
						}
					}
				}
			}
			void CLoader::DestroyDistantChunks(void)
			{
				while (!m_beginDestroyChunkThread);
				for (;;)
				{
					if (m_ended) break;

					chunk::Chunk::WCoordChunk wcc = PlayerWPosInChunkCoordinates();
					glm::vec2 vec2CastWCPlayerPos = glm::vec2(static_cast<float>(wcc.wpos.x), static_cast<float>(wcc.wpos.z));

					std::lock_guard<std::mutex> guard(cmap::dumutex);
					for (auto list = m_currentMap->Begin(); list != m_currentMap->End(); ++list)
					{
						for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
						{
						}

						for (auto c = list->begin(); c != list->end();)
						{
							for (; m_currentMap->UpdateState() == cmap::CMap::update_t::UPDATE_ACTIVE; )
							{
							}

							chunk::Chunk::WCoordChunk chunkWC = c->ChunkCoordinate();
							glm::vec2 vec2CastCWCChuPos = glm::vec2(static_cast<float>(chunkWC.wpos.x), static_cast<float>(chunkWC.wpos.z));

							if (glm::abs(glm::distance(vec2CastWCPlayerPos, vec2CastCWCChuPos)) > 13.0f)
							{
								std::lock_guard<std::mutex> guard(cmap::rumutex);
								std::cout << glm::distance(vec2CastWCPlayerPos, vec2CastCWCChuPos) << std::endl;
								c->DestroyGPUBuffer();
								list->erase(c++);
							}
							else ++c;
						}
					}
				}
			}
			CLoader::CLoader(cmap::CMap* cm, ent::Entity* player, int32_t seed, terrain::Terrain& t, structures::StructuresHandler& sh)
				: m_currentMap(cm), m_player(player), m_currentTerrain(&t), m_currentStructuresHandler(&sh), m_ended(false),
				m_beginDestroyChunkThread(false)
			{
			}
			// spawn thread
			void CLoader::Spawn(GLFWwindow* window)
			{
				m_playerCurrentChunkCoordinates = { { -0x551, -0x914} };
				// loads all the chunks under the player

				//m_clthread[0] = std::thread(&CLoader::UpdateChunksUnder, this,
					//[=](int32_t a, int32_t b) {return a < b; } /* conditional lambda */, [&](int32_t& x) { ++x; },/* incremental lambda */
					//[=](int32_t a, int32_t b) {return a < b; } /* conditional lambda */, [&](int32_t& z) { ++z; },/* incremental lambda */ -10, -10);

				// loads the chunks in the direction of the player
				m_clthread[0] = std::thread(&CLoader::UpdateChunksDistant, this);
				//
				
			//	m_clthread[1] = std::thread(&CLoader::DestroyDistantChunks, this);
				//m_clthread[1] = std::thread(&CLoader::DestroyDistantChunks, this);

				m_clthread[0].detach();
				//m_clthread[1].detach();
			}
			chunk::Chunk::WCoordChunk CLoader::PlayerWPosInChunkCoordinates(void)
			{
				glm::vec3 v = *m_player->EntityWorldPosition();
				WVec2 xz = { static_cast<int32_t>(v.x), static_cast<int32_t>(v.z) };
				int32_t x = xz.x == 0 ? 0 : (abs(xz.x) + 8) * (xz.x / abs(xz.x)) / 16;
				int32_t z = xz.z == 0 ? 0 : (abs(xz.z) + 8) * (xz.z / abs(xz.z)) / 16;
				return { { x, z } };
			}
		}
	}
}