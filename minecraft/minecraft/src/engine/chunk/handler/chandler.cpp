#include "chandler.h"
#include "../../../log.h"

namespace minecraft
{
	namespace chunk
	{
		CHandler::CHandler(signed int seed)
			: m_chunkMap(seed), m_seed(seed), m_terrain(seed)
		{
			Log("seed : ", m_seed);
		}
		void CHandler::Init(void)
		{
			ChunkMapInit();
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
			m_chunkloader = new loader::CLoader(&m_chunkMap, player, m_seed, m_terrain);
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
			chunk::Chunk::WCoordChunk wcc = CalculateChunkCoordinateOfWPos(wpos);
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
			signed int x = xz.x == 0 ? 0 : (abs(xz.x) + 8) * (xz.x / abs(xz.x)) / 16;
			signed int z = xz.z == 0 ? 0 : (abs(xz.z) + 8) * (xz.z / abs(xz.z)) / 16;
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
	}
}