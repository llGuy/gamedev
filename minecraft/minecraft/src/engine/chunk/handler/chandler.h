#ifndef CUNK_HANDLER_HEADER
#define CHUNK_HANDLER_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../data/common.h"
#include "../event/cevent.h"
#include "../map/cmap.h"
#include "../loader/chunk_loader.h"
#include "../../../shader/shprogram.h"
#include "../../terrain/terrain.h"
#include "../../block/block.h"

namespace minecraft
{
	namespace chunk
	{
		class CHandler
		{
		public:
			explicit CHandler(void) = default;
			explicit CHandler(int32_t seed);
		public:
			// initializations
			void Init(void);
			void UseSHProgram(void);
			void UseSHProgramLMesh(void);
			void AfterGLEWInit(void);
			void GetUniform(void);
		public:
			void RecieveChunkEvent(ChunkEventHandler::event_t e, const glm::vec3& p = glm::vec3(0.0f),
				const glm::vec3& d = glm::vec3(0.0f), const Block::block_t& b = Block::block_t::INV);
			float BlockUnderPoint(glm::vec3 wpos);
			float HighestBlock(glm::vec3 wpos);
			glm::vec3 BlockWPos(glm::vec3 wpos);
			data::CUDataLocs& Locations(void);
			data::CUDataLocs& LMeshLocations(void);
			cmap::CMap::iterator Begin(void);
			cmap::CMap::iterator End(void);
			void LaunchChunkLoader(ent::Entity* player, GLFWwindow* window);
			cmap::CMap::update_t MapUpdateState(void);
			const bool MapDeltedLLists(void);
			ChunkDB::CCorners ChunkCorners(Chunk::WCoordChunk wcc);
			void ResetMapDeletedLListsBool(void);
			WVec2 CalculateCoordsInChunks(const glm::vec2& worldxz);
			bool Obstruction(glm::vec3 flags, glm::vec3 wpos);
		private:
			void ChunkMapInit(void);
			void ChunkLoaderInit(ent::Entity* player, GLFWwindow* window);
			chunk::Chunk::WCoordChunk CalculateChunkCoordinateOfWPos(const glm::vec3& v) const;
			CVec2 CalculateBlockCoordInChunk(const chunk::Chunk::WCoordChunk& wcc, const glm::vec3& v) const;
			void SHInit(void);
		private:
			::sh::SHProgram m_chunkshprogram;
			::sh::SHProgram m_liquidMeshProgram;
			data::CUDataLocs m_udataloc;
			data::CUDataLocs m_udatalocLMesh;
			loader::CLoader* m_chunkloader;
			terrain::Terrain m_terrain;
			structures::StructuresHandler m_structHandler;
			ChunkEventHandler m_eventHandler;
			cmap::CMap m_chunkMap;
			int32_t m_seed;
		};
	}
}

#endif