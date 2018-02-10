#ifndef CUNK_HANDLER_HEADER
#define CHUNK_HANDLER_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../data/common.h"
#include "../loader/chunk_loader.h"
#include "../../../shader/shprogram.h"
#include "../biome/biome_handler.h"
#include "../map/cmap.h"

namespace minecraft
{
	namespace chunk
	{
		class CHandler
		{
		public:
			explicit CHandler(void) = default;
			explicit CHandler(signed int seed);
		public:
			// initializations
			void Init(void);
			void UseSHProgram(void);
			void AfterGLEWInit(void);
			void GetUniform(void);
		public:
			glm::vec3 BlockWPos(glm::vec3 wpos);
			data::CUDataLocs& Locations(void);
			cmap::CMap::iterator Begin(void);
			cmap::CMap::iterator End(void);
			void LaunchChunkLoader(ent::Entity* player, GLFWwindow* window);
			cmap::CMap::update_t MapUpdateState(void);
			const bool MapDeltedLLists(void);
			void ResetMapDeletedLListsBool(void);
		private:
			void ChunkMapInit(void);
			void ChunkLoaderInit(ent::Entity* player, GLFWwindow* window);
			WVec2 CalculateCoordsInChunks(const glm::vec2& worldxz);
			chunk::Chunk::WCoordChunk CalculateChunkCoordinateOfWPos(const glm::vec3& v) const;
			CVec2 CalculateBlockCoordInChunk(const chunk::Chunk::WCoordChunk& wcc, const glm::vec3& v) const;
			void SHInit(void);
		private:
			::sh::SHProgram m_chunkshprogram;
			data::CUDataLocs m_udataloc;
			loader::CLoader* m_chunkloader;
			biome::BiomeHandler m_biomeHandler;
			cmap::CMap m_chunkMap;
			signed int m_seed;
		};
	}
}

#endif