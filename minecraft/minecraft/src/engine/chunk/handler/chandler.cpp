#include "chandler.h"
#include "../../../log.h"

namespace minecraft
{
	namespace chunk
	{
		CHandler::CHandler(int32_t seed)
			: m_chunkMap(seed), m_seed(seed), m_terrain(seed), m_structHandler(seed)
		{
			Log("seed : ", m_seed);
		}
		void CHandler::Init(void)
		{
			ChunkMapInit();
		}
		float CHandler::BlockUnderPoint(glm::vec3 wpos)
		{
			wpos = glm::round(wpos);
			chunk::Chunk::WCoordChunk wcc = CalculateChunkCoordinateOfWPos(wpos);
			CVec2 blockChunkCoordinate = CalculateBlockCoordInChunk(wcc, wpos);
			// convert back to world coordinate space
			if (!m_chunkMap.Exists(wcc)) return 0.0f;
			float blockY = m_chunkMap[wcc].BlockUnder(wcc.wpos, blockChunkCoordinate, wpos, m_chunkMap[wcc].NegativeCornerWPos());
			
			if (static_cast<int32_t>(blockY) == -255)
			{
				m_chunkMap[wcc].BlockUnder(wcc.wpos, blockChunkCoordinate, wpos, m_chunkMap[wcc].NegativeCornerWPos());
			}

			return blockY;// +0.5f;
		}
		float CHandler::HighestBlock(glm::vec3 wpos)
		{
			// convert to backend coordinate space
			wpos = glm::round(wpos);
			chunk::Chunk::WCoordChunk wcc = CalculateChunkCoordinateOfWPos(wpos);
			CVec2 blockChunkCoordinate = CalculateBlockCoordInChunk(wcc, wpos);
			// convert back to world coordinate space
			if (!m_chunkMap.Exists(wcc)) return 0.0f;
			return m_chunkMap[wcc].HighestBlock(wcc.wpos, blockChunkCoordinate, wpos, m_chunkMap[wcc].NegativeCornerWPos()) + 0.5f;
		}
		void CHandler::UpdateBlockPointer(const glm::vec3& pdirection, const glm::vec3& pposition)
		{
			glm::vec3 p = m_eventHandler.PointingAt(pposition, pdirection, m_chunkMap);
			WVec2 chunkCoord = CalculateChunkCoordinateOfWPos(p).wpos;
			CVec2 blockCoord = CalculateBlockCoordInChunk({chunkCoord}, p);
			if (p.y < -511.0f)
			{
				m_blockPointer.NoBlocks();
			}
			else m_blockPointer.RecieveBlockPosition(p, blockCoord, chunkCoord);
		}
		void CHandler::RecieveChunkEvent(ChunkEventHandler::event_t e, const glm::vec3& p, const glm::vec3& d, const Block::block_t& b)
		{
			m_eventHandler.Event(e, m_chunkMap, m_terrain, m_blockPointer, p, d, b);
		}
		bool CHandler::Obstruction(glm::vec3 flags, glm::vec3 wpos)
		{
			enum
				: bool
			{
				NO_OBSTRUCTION = false,

				OBSTRUCTION = true
			};

			glm::vec3 rwpos = glm::round(wpos + flags);
			chunk::Chunk::WCoordChunk wcc = CalculateChunkCoordinateOfWPos(rwpos);
			CVec2 blockchunkCoordinate = CalculateBlockCoordInChunk(wcc, rwpos);

			if (!m_chunkMap.Exists(wcc)) return NO_OBSTRUCTION;
			else
			{
				if (fabs(flags.y) > 0.5f)
				{
					auto& c = m_chunkMap[wcc];
					bool exists = c.BlockExists(wcc.wpos, blockchunkCoordinate, rwpos);
					return exists;
				}
				else
				{
					auto& c = m_chunkMap[wcc];
					return (c.BlockExists(wcc.wpos, blockchunkCoordinate, rwpos) ||
						c.BlockExists(wcc.wpos, blockchunkCoordinate, rwpos + glm::vec3(0.0f, 1.0f, 0.0f))) &&
						glm::all(glm::lessThan(glm::abs(wpos - rwpos), glm::vec3(1.05f)));
				}
			}
			return false;
		}
		cmap::CMap::iterator CHandler::Begin(void)
		{
			return m_chunkMap.Begin();
		}
		cmap::CMap::iterator CHandler::End(void)
		{
			return m_chunkMap.End();
		}
		void CHandler::UseSHProgram(void)
		{
			m_chunkshprogram.UseProgram();
		}
		void CHandler::AfterGLEWInit(void)
		{
			m_chunkMap.AfterGLEWInit();
			m_blockPointer.CreateBuffer();
			SHInit();
		}
		void CHandler::LaunchChunkLoader(ent::Entity* player, GLFWwindow* window)
		{
			ChunkLoaderInit(player, window);
		}
		void CHandler::ChunkLoaderInit(ent::Entity* player, GLFWwindow* window)
		{
			m_chunkloader = new loader::CLoader(&m_chunkMap, player, m_seed, m_terrain, m_structHandler);
			m_chunkloader->Spawn(window);
		}
		void CHandler::SHInit(void)
		{
			// liquid
			std::vector<const char*> lattribs({"vertex_position"});
			m_liquidMeshProgram.Init("res\\shaders\\liquid\\vsh.shader", "res\\shaders\\liquid\\fsh.shader",
				"res\\shaders\\liquid\\gsh.shader");
			m_liquidMeshProgram.Compile();
			m_liquidMeshProgram.Link(lattribs);

			// blocks
			std::vector<const char*> battribs({ "vertex_position", "texture_data" });
			m_chunkshprogram.Init("res\\shaders\\block\\vsh.shader", "res\\shaders\\block\\fsh.shader",
				"res\\shaders\\block\\gsh.shader");
			m_chunkshprogram.Compile();
			m_chunkshprogram.Link(battribs);

			m_blockPointerSHP.Init("res\\shaders\\reg\\vsh.shader", "res\\shaders\\reg\\fsh.shader", "INV");
			std::vector<const char*> pattribs({ "vertex_position", "vertex_color" });
			m_blockPointerSHP.Compile();
			m_blockPointerSHP.Link(pattribs);
		}
		void CHandler::ChunkMapInit(void)
		{
			Log("generating chunks... ");
		}
		glm::vec3 CHandler::BlockWPos(glm::vec3 wpos)
		{
			chunk::Chunk::WCoordChunk wcc = CalculateChunkCoordinateOfWPos(wpos) ;
			CVec2 blockChunkCoordinate = CalculateBlockCoordInChunk(wcc, wpos);
			return m_chunkMap[wcc].BlockWorldCoord(blockChunkCoordinate, static_cast<int32_t>(wpos.y));
		}
		data::CUDataLocs& CHandler::Locations(void)
		{
			return m_udataloc;
		}
		WVec2 CHandler::CalculateCoordsInChunks(const glm::vec2& worldxz)
		{
			int32_t x = static_cast<int32_t>(worldxz.x);
			int32_t z = static_cast<int32_t>(worldxz.y);
			return { x / 16, z / 16 };
		}
		chunk::Chunk::WCoordChunk CHandler::CalculateChunkCoordinateOfWPos(const glm::vec3& v) const
		{
			WVec2 xz = { static_cast<int32_t>(v.x), static_cast<int32_t>(v.z) };

			int32_t x, z;

			auto ccoord = [&](int32_t a)->int32_t 
			{
				int32_t absa = abs(a);
				if (a > 0) return (abs((a + 8) % 16)) == 0 ? (a + 16) / 16 : (absa + 8) * (a / absa) / 16;
				else return a == 0 || (abs((a + 8) % 16)) == 0 ? a / 16 : (absa + 8) * (a / absa) / 16;
			};

			x = ccoord(xz.x);
			z = ccoord(xz.z);

			return { { x, z } };
		}
		CVec2 CHandler::CalculateBlockCoordInChunk(const chunk::Chunk::WCoordChunk& wcc, const glm::vec3& v) const
		{
			uint8_t x = static_cast<uint8_t>(wcc.wpos.x == 0 ? v.x + 8 :
				v.x - (wcc.wpos.x * 16 + 8 * (-wcc.wpos.x / wcc.wpos.x)));
			uint8_t z = static_cast<uint8_t>(wcc.wpos.z == 0 ? v.z + 8 :
				v.z - (wcc.wpos.z * 16 + 8 * (-wcc.wpos.z / wcc.wpos.z)));
			return { x , z };
		}
		void CHandler::GetUniform(void)
		{
			m_udataloc.projectionMatrixLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "projection_matrix");
			m_udataloc.viewMatrixLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "view_matrix");
			m_udataloc.lightPositionLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "light_position");
			m_udataloc.eyePositionLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "eye_position");
			m_udataloc.skyColorLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "sky_color");
			m_udataloc.modelMatrixLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "model_matrix");

			m_udatalocLMesh.projectionMatrixLocation = glGetUniformLocation(m_liquidMeshProgram.ProgramID(), "projection_matrix");
			m_udatalocLMesh.viewMatrixLocation = glGetUniformLocation(m_liquidMeshProgram.ProgramID(), "view_matrix");
			m_udatalocLMesh.lightPositionLocation = glGetUniformLocation(m_liquidMeshProgram.ProgramID(), "light_position");
			m_udatalocLMesh.eyePositionLocation = glGetUniformLocation(m_liquidMeshProgram.ProgramID(), "eye_position");
			m_udatalocLMesh.skyColorLocation = glGetUniformLocation(m_liquidMeshProgram.ProgramID(), "sky_color");
			m_udatalocLMesh.modelMatrixLocation = glGetUniformLocation(m_liquidMeshProgram.ProgramID(), "model_matrix");
			m_udatalocLMesh.liquidBlockTypeLocation = glGetUniformLocation(m_liquidMeshProgram.ProgramID(), "texture_data");

			m_udatalocBlockPointer.projectionMatrixLocation = glGetUniformLocation(m_blockPointerSHP.ProgramID(), "projection_matrix");
			m_udatalocBlockPointer.viewMatrixLocation = glGetUniformLocation(m_blockPointerSHP.ProgramID(), "view_matrix");
			m_udatalocBlockPointer.lightPositionLocation = glGetUniformLocation(m_blockPointerSHP.ProgramID(), "light_position");
			m_udatalocBlockPointer.eyePositionLocation = glGetUniformLocation(m_blockPointerSHP.ProgramID(), "eye_position");
			m_udatalocBlockPointer.skyColorLocation = glGetUniformLocation(m_blockPointerSHP.ProgramID(), "sky_color");
			m_udatalocBlockPointer.modelMatrixLocation = glGetUniformLocation(m_blockPointerSHP.ProgramID(), "model_matrix");
		}
		cmap::CMap::update_t CHandler::MapUpdateState(void)
		{
			return m_chunkMap.UpdateState();
		}
		const bool CHandler::MapDeltedLLists(void)
		{
			return m_chunkMap.DeletedLLists();
		}
		void CHandler::ResetMapDeletedLListsBool(void)
		{
			m_chunkMap.ResetDeltedLListsBool();
		}
		ChunkDB::CCorners CHandler::ChunkCorners(Chunk::WCoordChunk wcc)
		{
			return m_chunkMap[wcc].ChunkCorners();
		}
		void CHandler::UseSHProgramLMesh(void)
		{
			m_liquidMeshProgram.UseProgram();
		}
		void CHandler::UseBlockPointerProgram(void)
		{
			m_blockPointerSHP.UseProgram();
		}
		data::CUDataLocs& CHandler::LMeshLocations(void)
		{
			return m_udatalocLMesh;
		}
		data::CUDataLocs& CHandler::BlockPointerLocations(void)
		{
			return m_udatalocBlockPointer;
		}
		void* CHandler::BlockPointerOffset(void)
		{
			return m_blockPointer.Offset();
		}
		BlockPointer& CHandler::BPointer(void)
		{
			return m_blockPointer;
		}
		
	}
}