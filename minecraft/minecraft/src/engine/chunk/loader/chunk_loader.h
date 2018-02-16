#ifndef CHUNK_LOADER_HEADER
#define CHUNK_LOADER_HEADER

#include "../map/cmap.h"
#include "../../entities/player/player.h"
#include "../../terrain/terrain.h"
#include "../../structures/handler/structures_handler.h"

#include <thread>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

namespace minecraft
{
	namespace chunk
	{
		namespace loader
		{
			class CLoader
			{
			public:
				struct ChunkViewDirection
				{
					signed int x, z;
					const bool operator==(const ChunkViewDirection& other)
					{
						return (x == other.x && z == other.z);
					}
				};
				explicit CLoader(void) = default;
				CLoader& operator=(const CLoader&) = default;
				explicit CLoader(cmap::CMap* cm, ent::Entity* player, signed int seed, terrain::Terrain& t, structures::StructuresHandler& sh);
				//CLoader& operator=(const CLoader&& cl);
			public:
				void UpdateChunksUnder(std::function<bool(int, int)> x, std::function<void(int&)> xi,
					std::function<bool(int, int)> z, std::function<void(int&)> zi,
					int startx, int startz);
				void UpdateChunksDistant(void);
				// spawn thread
				void Spawn(GLFWwindow* window);
				chunk::Chunk::WCoordChunk PlayerWPosInChunkCoordinates(void);
			private:
				std::thread m_clthread[2];
				cmap::CMap* m_currentMap;
				terrain::Terrain* m_currentTerrain;
				structures::StructuresHandler* m_currentStructuresHandler;

				ent::Entity* m_player;
				
				chunk::Chunk::WCoordChunk m_playerCurrentChunkCoordinates;
				signed int m_seed;
			};
		}
	}
}

#endif