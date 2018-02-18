#include "chandler.h"
#include "../../../log.h"

namespace minecraft
{
	namespace chunk
	{
		CHandler::CHandler(signed int seed)
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
			
			if (static_cast<int>(blockY) == -255)
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

			//std::cout << wcc.wpos.x << ", " << wcc.wpos.z << std::endl; // "\t\t" <<
				//static_cast<unsigned int>(blockchunkCoordinate.x) << ", " << static_cast<unsigned int>(blockchunkCoordinate.z) << std::endl;

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
			std::vector<const char*> attribs({ "vertex_position", "texture_data" });
			m_chunkshprogram.Init("res\\shaders\\block\\vsh.shader", "res\\shaders\\block\\fsh.shader",
				"res\\shaders\\block\\gsh.shader");
			m_chunkshprogram.Compile();
			m_chunkshprogram.Link(attribs);
		}
		void CHandler::ChunkMapInit(void)
		{
			Log("generating chunks... ");
		}
		glm::vec3 CHandler::BlockWPos(glm::vec3 wpos)
		{
			chunk::Chunk::WCoordChunk wcc = CalculateChunkCoordinateOfWPos(wpos) ;
			CVec2 blockChunkCoordinate = CalculateBlockCoordInChunk(wcc, wpos);
			return m_chunkMap[wcc].BlockWorldCoord(blockChunkCoordinate, static_cast<signed int>(wpos.y));
		}
		data::CUDataLocs& CHandler::Locations(void)
		{
			return m_udataloc;
		}
		WVec2 CHandler::CalculateCoordsInChunks(const glm::vec2& worldxz)
		{
			signed int x = static_cast<signed int>(worldxz.x);
			signed int z = static_cast<signed int>(worldxz.y);
			return { x / 16, z / 16 };
		}
		chunk::Chunk::WCoordChunk CHandler::CalculateChunkCoordinateOfWPos(const glm::vec3& v) const
		{
			WVec2 xz = { static_cast<signed int>(v.x), static_cast<signed int>(v.z) };

			signed int x, z;

			auto ccoord = [&](signed int a)->signed int 
			{
				signed int absa = abs(a);
				if (a > 0) return (abs((a + 8) % 16)) == 0 ? (a + 16) / 16 : (absa + 8) * (a / absa) / 16;
				else return a == 0 || (abs((a + 8) % 16)) == 0 ? a / 16 : (absa + 8) * (a / absa) / 16;
			};

			x = ccoord(xz.x);
			z = ccoord(xz.z);

			return { { x, z } };
		}
		CVec2 CHandler::CalculateBlockCoordInChunk(const chunk::Chunk::WCoordChunk& wcc, const glm::vec3& v) const
		{
			unsigned char x = wcc.wpos.x == 0 ? static_cast<unsigned char>(v.x + 8) :
				static_cast<unsigned char>(v.x - (wcc.wpos.x * 16 + 8 * (-wcc.wpos.x / wcc.wpos.x)));
			unsigned char z = wcc.wpos.z == 0 ? static_cast<unsigned char>(v.z + 8) :
				static_cast<unsigned char>(v.z - (wcc.wpos.z * 16 + 8 * (-wcc.wpos.z / wcc.wpos.z)));
			return { x , z };
		}
		void CHandler::GetUniform(void)
		{
			m_udataloc.projectionMatrixLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "projection_matrix");
			m_udataloc.viewMatrixLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "view_matrix");
			m_udataloc.lightPositionLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "light_position");
			m_udataloc.eyePositionLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "eye_position");
			m_udataloc.skyColorLocation = glGetUniformLocation(m_chunkshprogram.ProgramID(), "sky_color");
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
	}
}