#ifndef CHUNK_DATA_BASE_HEADER
#define CHUNK_DATA_BASE_HEADER

#include <unordered_map>

#include "../../utility/glm_vecio.h"
#include "../../block/block.h"
#include "../gpu/gpuhandler/chunk_gpu_side_handler.h"
#include "../cdata/cvec2.h"
#include "../../terrain/terrain.h"
#include "../../terrain/terrain.h"
#include "blockystrip.h"
#include "../../structures/handler/structures_handler.h"

namespace minecraft
{
	namespace chunk
	{
		class ChunkDB
		{
		public:
			explicit ChunkDB(int32_t seed)
				: m_numBlocks(0), m_transparentBlocks(0)
			{
			}
		public:
			struct CCorners
			{
				glm::vec2 nn;
				glm::vec2 np;
				glm::vec2 pn;
				glm::vec2 pp;
			};
			void Load(WVec2 chunkCoords, WVec2 negCorner, terrain::Terrain& t);
			// chunk mesh
			enum class chunkExtr_t
				: uint32_t
			{
				NEG_X = 0,

				POS_X = 1,

				POS_Z = 2,

				NEG_Z = 3
			};
			void LoadStructures(WVec2 chunkCoords, structures::StructuresHandler& sh, WVec2 negCorner, terrain::Terrain& t);
			// for load visible, the gradient vectors of each biome need to be calculated
			// however, to avoid having to recalculate it over and over, this struct
			// saves the gradient vectors that have been calculated
			struct GVectorStructUtility
			{
				struct GVectorStruct
				{
					pnoise::PNoise::GradientVectors gv;
					bool computed = false;
				} gvs[static_cast<uint32_t>(biome::biome_t::INV)];

				GVectorStruct& GV(const biome::biome_t& b, pnoise::PNoise::CellCorners& corners, terrain::Terrain& t)
				{
					uint32_t biomeGradientVectorIndex = static_cast<uint32_t>(b);
					if (!(gvs[biomeGradientVectorIndex].computed))
					{
						gvs[biomeGradientVectorIndex].computed = true;
						gvs[biomeGradientVectorIndex].gv = t.GVectors(corners, terrain::Terrain::choice_t::HM, b);
					}
					return gvs[biomeGradientVectorIndex];
				}
			};
			void LoadTop(WVec2 chunkCoords, WVec2 negCorner, terrain::Terrain& t);
			// data for checking if neighbouring chunks need to load blocks
			struct DBNeighbourChunkData 
			{
				// quantity
				uint8_t quant;
				struct
				{
					WCoordChunk wcc;
					CVec2 cv;
					int32_t y;
				} offsets[2];	// maximum 2

				void App(const WCoordChunk& wcc, CVec2 c, int32_t y, const WVec2 offset)
				{
					// to avoid the coords getting below 0 so that if
					// -1 will go to 15 and 15 % 16 is 15
					c.x += 16 + offset.x;	c.z += 16 + offset.z;
					c.x %= 16;				c.z %= 16;
					WCoordChunk newWcc = { {wcc.wpos.x + offset.x, wcc.wpos.z + offset.z} };
					offsets[quant] = { newWcc, c, y };
					++quant;
				}
			};
			DBNeighbourChunkData DestroyBlock(CVec2 bcoord, float y, terrain::Terrain& t, WVec2 negCorner, WCoordChunk wcc);
			void LoadGPUBuffer(void);
			void DestroyHEAPMemoryForBlocksWPos(void);
			void LoadVisibleBlock(CVec2 c, int32_t y,
				terrain::Terrain& t, WVec2 negCorner, WVec2& wcc);
		public:
			void PlaceBlock(CVec2 c, float y, WVec2 negCorner, WCoordChunk wcc, const Block::block_t& b);
			void LoadAllNeighbouringVisibleBlocks(CVec2 c, int32_t y, terrain::Terrain& t,
				WVec2 negCorner, WCoordChunk& wcc, DBNeighbourChunkData& d);
			void LoadNeighbouringVisibleBlock(CVec2 c, int32_t y, BlockYStrip& bys,
				terrain::Terrain& t, WVec2 negCorner, WCoordChunk& wcc);
			
			/* getter */
			const bool BlockExists(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos);
			const bool BlockExists(WVec2 chunkCoord, CVec2 ccoord, int32_t y);
			int32_t HightestBlock(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos);
			int32_t BlockUnder(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos);
			glm::vec3 WCoord(WVec2 wchunkCoord, CVec2 ccoord, int32_t elevation, const WVec2&& negativeCornerWPos);
			const Block::block_t BlockType(CVec2 ccoord, int32_t elevation);
			uint8_t Index(CVec2 ccoord) const;
			::std::size_t NumBlocks(void);
			VAO* Vao(void);
			const bool CreatedVAO(void);
			CCorners& ChunkCorners(void);
			
			void DestroyChunkData(void);

		private:
			bool Destroyed(CVec2 c, int32_t y);
			void UpdateIndices(std::array<uint32_t, 16>& indices);
			void LoadGPUData(WVec2 chunkCoords, WVec2 negCorner);
			void LoadTopGPUData(WVec2 chunkCoords, WVec2 negCorner);
			void GradLoadGPUData(WVec2 chunkCoord, WVec2 negCorner, uint8_t x, uint8_t z, BlockYStrip& bys, int32_t y);
			bool BlockIsVisible(uint8_t x, int32_t y, uint8_t z, const BlockYStrip& bys);
			CCoord CompressChunkCoord(CVec2 cc) const;
			void GenerateCorners(WVec2 negCorner);
			int32_t Height(const WVec2& negCorner, const uint8_t& x, const uint8_t& z,
				int32_t mh, pnoise::PNoise::CellCorners& cc, pnoise::PNoise::GradientVectors& gv, terrain::Terrain& t, biome::BiomeData b);

			// for the LoadTop() for the neighbouring biomes
			biome::BiomeData DetermineNeighbouringBiome(terrain::Terrain& t,
				const chunkExtr_t chunkExtr, const WVec2& chunkCoords, const WVec2& negCorner, uint8_t x, uint8_t z);
			const bool AtExtr0(uint8_t xzInChunk);
			const bool AtExtr15(uint8_t xzInChunk);
			const bool AtHeightmapExtrN(const int32_t xzWorld);
			const bool AtHeightmapExtrP(const int32_t xzWorld);
			void NeighbouringBiomes(biome::BiomeData* nb, const WVec2& chunkCoords,
				terrain::Terrain& t, const WVec2& negCorner,
				const uint8_t x, const uint8_t z, pnoise::PNoise::CellCorners& bcc,
				pnoise::PNoise::GradientVectors& gv);
			pnoise::PNoise::CellCorners NeighbouringHeightmapCellCorners(terrain::Terrain& t, WVec2& neighbouringchunkcoord);
			pnoise::PNoise::GradientVectors NeighbouringHeightmapCellGVectors(pnoise::PNoise::CellCorners c,
				terrain::Terrain& t);
			
			signed int DetermineNeighbouringHeight(biome::BiomeData* nb, terrain::Terrain& t, GVectorStructUtility gvs,
				const chunkExtr_t ce, const WVec2& negCorner, uint8_t xInChunk, uint8_t zInChunk, WVec2& chunkCoord);
			void NeighbouringHeights(biome::BiomeData* nb, terrain::Terrain& t, const WVec2& negCorner,
				int32_t* nh, uint8_t xInChunk, uint8_t zInChunk, WVec2& chunkCoord, GVectorStructUtility);
			int32_t SmallestNeighbour(int32_t* nh);
			void AppendBlock(uint8_t x, uint8_t z, int32_t y,
				terrain::Terrain& t, biome::biome_t b, int32_t bysH, BlockYStrip& bys);
			Block& AppendBlock(uint8_t x, uint8_t z, int32_t y,
				BlockYStrip& bys, Block::block_t b);
			void GenerateHeightmapCellCorners(const WVec2& chunkCoord, terrain::Terrain& t);
			void GenerateHeightmapCellGVectors(terrain::Terrain& t);
		private:
			gpu::CGPUHandler m_gpuh;
			// heightmap cell corners
			pnoise::PNoise::CellCorners m_corners;
			// chunk corners
			CCorners m_ccorners;
			// heightmap cell gradient vectors
			pnoise::PNoise::GradientVectors m_gradientVectors;
			BlockYStrip m_blocks[16 * 16];
			uint32_t m_numBlocks;
			uint32_t m_transparentBlocks;
		};
	}
}
#endif