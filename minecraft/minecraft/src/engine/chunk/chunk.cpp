#include "chunk.h"

namespace minecraft
{
	namespace chunk
	{
		Chunk::Chunk(void)
			: m_dataBase(0), m_loaded(false), m_gpubufferloaded(false)
		{
		}
		Chunk::Chunk(const WCoordChunk& wcoordChunk, int32_t seed, terrain::Terrain& t, structures::StructuresHandler& sh)
			: m_wcoordChunk(wcoordChunk), m_dataBase(seed), m_loaded(false), m_gpubufferloaded(false)
		{
			LoadTop(t, sh);
			LoadStructures(t, sh);
		}

		Chunk::Chunk(const WCoordChunk&& wcoordChunk, int32_t seed, terrain::Terrain& t, structures::StructuresHandler& sh)
			: m_wcoordChunk(wcoordChunk), m_dataBase(seed), m_loaded(false), m_gpubufferloaded(false)
		{
			LoadTop(t, sh);
			LoadStructures(t, sh);
		}
		void Chunk::DestroyBlock(CVec2 c, float y, terrain::Terrain& t)
		{
			m_dataBase.DestroyBlock(c, y, t, NegativeCornerWPos(), ChunkCoordinate().wpos);
			m_gpubufferloaded = false;	// requests to be reloaded
		}

		void Chunk::LoadAll(terrain::Terrain& t, structures::StructuresHandler& sh)
		{
			m_dataBase.Load(m_wcoordChunk.wpos, NegativeCornerWPos(), t);
			m_loaded = true;
		}

		void Chunk::LoadTop(terrain::Terrain& t, structures::StructuresHandler& sh)
		{
			m_dataBase.LoadTop(m_wcoordChunk.wpos, NegativeCornerWPos(), t);
			m_loaded = true;
		}

		void Chunk::LoadStructures(terrain::Terrain& t, structures::StructuresHandler& sh)
		{
			m_dataBase.LoadStructures(m_wcoordChunk.wpos, sh, NegativeCornerWPos(), t);
		}

		void Chunk::LoadGPUBuffer(void)
		{			
			m_dataBase.LoadGPUBuffer();
			m_gpubufferloaded = true;
		}

		void Chunk::DestroyHEAPMemoryForBlocksWPos(void)
		{
			m_dataBase.DestroyHEAPMemoryForBlocksWPos();
		}

		glm::vec3 Chunk::BlockWorldCoord(const CVec2 cc, int32_t elevation)
		{
			return m_dataBase.WCoord(m_wcoordChunk.wpos, cc, elevation, NegativeCornerWPos());
		}

		const bool Chunk::BlockExists(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos)
		{
			return m_dataBase.BlockExists(chunkCoord, ccoord, wpos);
		}

		Block::block_t Chunk::BlockType(const CVec2 cc, int32_t elevation)
		{
			return m_dataBase.BlockType(cc, elevation);
		}

		Chunk::WCoordChunk Chunk::ChunkCoordinate(void)
		{
			return m_wcoordChunk;
		}

		WVec2 Chunk::NegativeCornerWPos(void) const
		{
			return { m_wcoordChunk.wpos.x * 16 - 8, m_wcoordChunk.wpos.z * 16 - 8 };
		}

		::std::size_t Chunk::NumBlocks(void)
		{
			return m_dataBase.NumBlocks();
		}

		VAO* Chunk::Vao(void)
		{
			return m_dataBase.Vao();
		}

		const bool Chunk::Loaded(void)
		{
			return m_loaded;
		}

		const bool Chunk::BufferLoaded(void)
		{
			return m_gpubufferloaded;
		}

		const bool Chunk::CreatedVAO(void)
		{
			return m_dataBase.CreatedVAO();
		}

		float Chunk::HighestBlock(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos)
		{
			return static_cast<float>(m_dataBase.HightestBlock(chunkCoord, ccoord, wpos, negativeCornerWPos));
		}
		float Chunk::BlockUnder(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos)
		{
			return static_cast<float>(m_dataBase.BlockUnder(chunkCoord, ccoord, wpos, negativeCornerWPos));
		}
		ChunkDB::CCorners Chunk::ChunkCorners(void)
		{
			return m_dataBase.ChunkCorners();
		}
	}
}