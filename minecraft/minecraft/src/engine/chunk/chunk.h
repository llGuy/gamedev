#ifndef CHUNK_HEADER
#define CHUNK_HEADER

#include "../block/block.h"
#include "database/chunk_data_base.h"
#include "biome/plains/plains.h"
#include "noise/regular/reg_perlin_noise.h"
#include "gpu/gpuhandler/chunk_gpu_side_handler.h"

namespace minecraft
{
	namespace chunk
	{
		class Chunk
		{
		public:
			struct WCoordChunk
			{
				WCoordChunk(void) = default;
				WCoordChunk(const WVec2& v)
					: wpos(v) {}
				WVec2 wpos;
				bool operator==(const WCoordChunk& c)
				{
					return (c.wpos.x == wpos.x && c.wpos.z == wpos.z);
				}
				bool operator!=(const WCoordChunk& c)
				{
					return (c.wpos.x != wpos.x || c.wpos.z != wpos.z);
				}
			};
			// assignment operator
			Chunk& operator=(const Chunk& c)
			{
				this->m_wcoordChunk = c.m_wcoordChunk;
				this->m_dataBase = c.m_dataBase;
				this->m_loaded = c.m_loaded;
				this->m_gpubufferloaded = c.m_gpubufferloaded;

				return *this;
			}

			explicit Chunk(void);
			explicit Chunk(signed int seed) : m_wcoordChunk({ {0, 0} }), m_dataBase(seed), 
				m_loaded(false), m_gpubufferloaded(false) {}
			explicit Chunk(const WCoordChunk& wcoordChunk, signed int seed, biome::BiomeHandler& bh);
			explicit Chunk(const WCoordChunk&& wcoordChunk, signed int seed, biome::BiomeHandler& bh);
			void LoadGPUBuffer(void);
			void LoadAll(biome::BiomeHandler& bh);
		public:
			/* getters */
			glm::vec3 BlockWorldCoord(const CVec2 cc, signed int elevation);
			WCoordChunk ChunkCoordinate(void);
			Block::block_t BlockType(const CVec2 cc, signed int elevation);
			::std::size_t NumBlocks(void);
			VAO* Vao(void);
			const bool Loaded(void);
			const bool BufferLoaded(void);
			const bool CreatedVAO(void);
		private:
			void LoadTop(biome::BiomeHandler& bh);
			WVec2 NegativeCornerWPos(void) const;
		private:
			WCoordChunk m_wcoordChunk;
			ChunkDB m_dataBase;
			bool m_loaded;
			bool m_gpubufferloaded;
		};
	}
}

#endif