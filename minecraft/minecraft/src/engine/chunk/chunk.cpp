#include "chunk.h"
#include "biome/biome_handler.h"

namespace minecraft
{
	namespace chunk
	{
		Chunk::Chunk(void)
			: m_dataBase(0), m_loaded(false), m_gpubufferloaded(false)
		{
		}
		Chunk::Chunk(const WCoordChunk& wcoordChunk, signed int seed, biome::BiomeHandler& bh)
			: m_wcoordChunk(wcoordChunk), m_dataBase(seed), m_loaded(false), m_gpubufferloaded(false)
		{
			LoadTop(bh);
		}

		Chunk::Chunk(const WCoordChunk&& wcoordChunk, signed int seed, biome::BiomeHandler& bh)
			: m_wcoordChunk(wcoordChunk), m_dataBase(seed), m_loaded(false), m_gpubufferloaded(false)
		{
			LoadTop(bh);
		}

		void Chunk::LoadAll(biome::BiomeHandler& bh)
		{
			m_dataBase.Load(m_wcoordChunk.wpos, NegativeCornerWPos(), bh);
			m_loaded = true;
		}

		void Chunk::LoadTop(biome::BiomeHandler& bh)
		{
			m_dataBase.LoadTop(m_wcoordChunk.wpos, NegativeCornerWPos(), bh);
			m_loaded = true;
		}

		void Chunk::LoadGPUBuffer(void)
		{			
			m_dataBase.LoadGPUBuffer();
			m_gpubufferloaded = true;
		}

		glm::vec3 Chunk::BlockWorldCoord(const CVec2 cc, signed int elevation)
		{
			return m_dataBase.WCoord(m_wcoordChunk.wpos, cc, elevation, NegativeCornerWPos());
		}

		Block::block_t Chunk::BlockType(const CVec2 cc, signed int elevation)
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
	}
}