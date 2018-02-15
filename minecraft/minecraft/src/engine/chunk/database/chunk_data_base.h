#ifndef CHUNK_DATA_BASE_HEADER
#define CHUNK_DATA_BASE_HEADER

#include <unordered_map>

#include "../../utility/glm_vecio.h"
#include "../../block/block.h"
#include "../gpu/gpuhandler/chunk_gpu_side_handler.h"
#include "../cdata/cvec2.h"
#include "../noise/regular/reg_perlin_noise.h"
#include "../../terrain/terrain.h"
#include "../../terrain/terrain.h"
#include "blockystrip.h"

namespace minecraft
{
	namespace chunk
	{
		class ChunkDB
		{
		public:
			explicit ChunkDB(signed int seed)
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
			void Load(WVec2 chunkCoords, WVec2 negCorner, terrain::Terrain& t)
			{
				pnoise::PNoise::CellCorners bcc = t.CellCorners(chunkCoords, terrain::Terrain::choice_t::BM);
				pnoise::PNoise::GradientVectors gv = t.GVectors(bcc, terrain::Terrain::choice_t::HM);

				GenerateHeightmapCellCorners(chunkCoords, t);
				GenerateHeightmapCellGVectors(t);

				for (signed int z = 0; z < 16; ++z)
				{
					for (signed int x = 0; x < 16; ++x)
					{
						//biome::BiomeMap::DifferenceVectors dv = t.DVectors(glm::vec2(negCorner.x + x, negCorner.z + z), bcc, terrain::Terrain::choice_t::HM);
						//biome::biome_t b = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x, negCorner.z + z)), bcc, gv);
						//float height = Height(negCorner, x, z, bh.MaxBiomeHeight(b), m_corners, m_gradientVectors) + 20;
						//for (signed int y = -30; y < static_cast<signed int>(height); ++y)
						//{
						//	CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
						//	BlockYStrip& bys = m_blocks[Index(cc)];
						//	bys.ystrip[y] = Block(CompressChunkCoord(cc), bh.BlockType(b, bh.MaxBiomeHeight(b), y));
						//}
					}
				}
				LoadGPUData(chunkCoords, negCorner);
			}
			// chunk mesh
			enum class chunkExtr_t
				: unsigned int
			{
				NEG_X = 0,

				POS_X = 1,

				POS_Z = 2,

				NEG_Z = 3
			};
			void LoadTop(WVec2 chunkCoords, WVec2 negCorner, terrain::Terrain& t)
			{
				GenerateCorners(negCorner);

				pnoise::PNoise::CellCorners bcc = t.CellCorners(chunkCoords, terrain::Terrain::choice_t::BM);
				pnoise::PNoise::GradientVectors gv = t.GVectors(bcc, terrain::Terrain::choice_t::HM);

				GenerateHeightmapCellCorners(chunkCoords, t);
				GenerateHeightmapCellGVectors(t);

				for (signed int z = 0; z < 16; ++z)
				{
					for (signed int x = 0; x < 16; ++x)
					{
						// biome of the current block
						glm::vec2 blockWCoord = glm::vec2(negCorner.x + x, negCorner.z + z);
						biome::biome_t b = t.Biome(blockWCoord, bcc, gv);

						// calculating neibouring biomes
						biome::biome_t neighbouringBiomes[4];
						NeighbouringBiomes(neighbouringBiomes, chunkCoords, t, negCorner, x, z, bcc, gv);

						signed int neighbouringHeights[4];
						NeighbouringHeights(neighbouringBiomes, t, negCorner, neighbouringHeights, x, z, chunkCoords);

						// height of current block
						signed int h = Height(negCorner, x, z, t.BiomeMaxHeight(b), m_corners, m_gradientVectors, t) + t.BiomeOffset(b);

						// calculating smallest height
						signed int smallestNeighbour = SmallestNeighbour(neighbouringHeights);

						BlockYStrip& bys = m_blocks[(Index({ static_cast<unsigned char>(x), static_cast<unsigned char>(z) }))];
						// regardless of whether at corner of chunk
						for (signed int y = h; y >= smallestNeighbour; --y) AppendBlock(x, z, y, t, b, h, bys);
						bys.smallest = smallestNeighbour;
						if (h < smallestNeighbour)
						{
							AppendBlock(x, z, h, t, b, h, bys);
							bys.smallest = h;
						}
					}
				}
				LoadGPUData(chunkCoords, negCorner);
			}
			void LoadGPUBuffer(void)
			{
				m_gpuh.LoadGPUBuffer();
			}
			void DestroyHEAPMemoryForBlocksWPos(void)
			{
				m_gpuh.DestroyVector();
			}
		public:
			/* getter */
			const bool BlockExists(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos)
			{
				signed int y = static_cast<signed int>(wpos.y);
				unsigned int index = Index(ccoord);
				// if not equal then the block exists
				return m_blocks[index].ystrip.find(y) != m_blocks[index].ystrip.end();
			}
			signed int HightestBlock(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos)
			{
				signed int smallest = 0xfffff;
				for (auto& bys : m_blocks[Index(ccoord)].ystrip)
				{
					if (smallest > bys.first) smallest = bys.first;
				}
				return smallest;
			}
			signed int BlockUnder(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos)
			{
				signed int y = static_cast<signed int>(wpos.y + 1.0f);
				unsigned int index = Index(ccoord);
				auto& bys = m_blocks[index];
				for (; y >= bys.smallest; --y)
				{
					if (bys.ystrip.find(y) != bys.ystrip.end()) return y;
				}
				/* if there aren't any blocks under the player, function will return -0xff */
				return -0xff;
			}
			glm::vec3 WCoord(WVec2 wchunkCoord, CVec2 ccoord, signed int elevation, const WVec2&& negativeCornerWPos)
			{
				return m_blocks[Index(ccoord)].ystrip[elevation].WPos(wchunkCoord, elevation, negativeCornerWPos);
			}
			const Block::block_t BlockType(CVec2 ccoord, signed int elevation)
			{
				return m_blocks[Index(ccoord)].ystrip[elevation].BlockType();
			}
			unsigned char Index(CVec2 ccoord) const
			{
				return ccoord.x + ccoord.z * 16;
			}
			::std::size_t NumBlocks(void)
			{
				return m_gpuh.NumBlocks();
			}
			VAO* Vao(void)
			{
				return m_gpuh.Vao();
			}
			const bool CreatedVAO(void)
			{
				return m_gpuh.CreatedVAO();
			}
			CCorners& ChunkCorners(void)
			{
				return m_ccorners;
			}
		private:
			void LoadGPUData(WVec2 chunkCoords, WVec2 negCorner)
			{
				m_gpuh.PrepareVector();
				for (signed int z = 0; z < 16; ++z)
				{
					for (signed int x = 0; x < 16; ++x)
					{
						CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
						BlockYStrip& bys = m_blocks[Index(cc)];
						for (auto& i : bys.ystrip)
						{
							if (BlockIsVisible(x, i.first, z, bys))
								m_gpuh.Load(&bys, Index(cc), i.first, chunkCoords, negCorner);
						}
					}
				}
			}
			void LoadTopGPUData(WVec2 chunkCoords, WVec2 negCorner)
			{
				for (signed int z = 0; z < 16; ++z)
				{
					for (signed int x = 0; x < 16; ++x)
					{
						CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
						BlockYStrip& bys = m_blocks[Index(cc)];
						for (auto& i : bys.ystrip)
						{
							m_gpuh.Load(&bys, Index(cc), i.first, chunkCoords, negCorner);
						}
					}
				}
			}
			bool BlockIsVisible(signed int x, signed int y, signed int z, const BlockYStrip& bys)
			{
				// is at the edge of the chunk
				if (x == 15 || z == 15 || x == 0 || z == 0) return true;
				// check x axis
				CVec2 cc = { static_cast<unsigned char>(x + 1), static_cast<unsigned char>(z) };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;
				cc = { static_cast<unsigned char>(x - 1), static_cast<unsigned char>(z) };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;

				cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z + 1) };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;
				cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z - 1) };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;

				// check y axis
				cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
				if (m_blocks[Index(cc)].ystrip.find(y + 1) == m_blocks[Index(cc)].ystrip.end()) return true;
				if (m_blocks[Index(cc)].ystrip.find(y - 1) == m_blocks[Index(cc)].ystrip.end()) return true;

				else return false;
			}
			CCoord CompressChunkCoord(CVec2 cc) const
			{
				unsigned char x = static_cast<unsigned char>(cc.x);
				unsigned char z = static_cast<unsigned char>(cc.z);
				return { static_cast<unsigned char>((x << 4) + z) };
			}
			void GenerateCorners(WVec2 negCorner)
			{
				glm::vec2 negativeCorner = glm::vec2(static_cast<float>(negCorner.x), static_cast<float>(negCorner.z));

				m_ccorners.nn = glm::vec2(negativeCorner.x - 0.5f,
					negativeCorner.y - 0.5f);
				m_ccorners.np = glm::vec2(negativeCorner.x - 0.5f,
					negativeCorner.y + 15.5f);
				m_ccorners.pn = glm::vec2(negativeCorner.x + 15.5f,
					negativeCorner.y - 0.5f);
				m_ccorners.pp = glm::vec2(negativeCorner.x + 15.5f,
					negativeCorner.y + 15.5f);
			}
			signed Height(const WVec2& negCorner, const signed int& x, const signed int& z, 
				signed int mh, pnoise::PNoise::CellCorners& cc, pnoise::PNoise::GradientVectors& gv, terrain::Terrain& t)
			{
				float blockx = static_cast<float>(negCorner.x + x);
				float blockz = static_cast<float>(negCorner.z + z);
				glm::vec2 blockxzworld = glm::vec2(blockx, blockz);
				return t.Height(blockxzworld, cc, gv, mh);
			}

			// for the LoadTop() for the neighbouring biomes
			biome::biome_t DetermineNeighbouringBiome(terrain::Terrain& t, 
				const chunkExtr_t chunkExtr, const WVec2& chunkCoords, const WVec2& negCorner, signed int x, signed int z)
			{
				static signed int offsetx[4]
				{
					-1, 1, 0, 0
				};
				static signed int offsetz[4]
				{
					0, 0, 1, -1
				};

				pnoise::PNoise::CellCorners bccnx = t.CellCorners(WVec2{
					chunkCoords.x + offsetx[static_cast<unsigned int>(chunkExtr)], 
					chunkCoords.z + offsetz[static_cast<unsigned int>(chunkExtr)] }, terrain::Terrain::choice_t::BM);
				pnoise::PNoise::GradientVectors gvnx = t.GVectors(bccnx, terrain::Terrain::choice_t::BM);
				return t.Biome(glm::vec2(negCorner.x + x + offsetx[static_cast<unsigned int>(chunkExtr)], 
					negCorner.z + z + offsetz[static_cast<unsigned int>(chunkExtr)]), bccnx, gvnx);
			}
			const bool AtExtr0(signed int xz)
			{
				//signed int extrn = static_cast<signed int>(m_corners.nn.x + 0.5f);
				//signed int extrp = static_cast<signed int>(m_corners.pp.x - 0.5f);
				return xz == 0 && xz != 15;
			}
			const bool AtExtr15(signed int xz)
			{
				//signed int extrn = static_cast<signed int>(m_corners.nn.x + 0.5f);
				//signed int extrp = static_cast<signed int>(m_corners.pp.x - 0.5f);
				return xz == 15 && xz != 0;
			}
			const bool AtHeightmapExtrN(const int xz)
			{
				signed int extrn = static_cast<signed int>(m_corners.nn.x + 0.5f);
				signed int extrp = static_cast<signed int>(m_corners.pp.x - 0.5f);
				return xz == extrn && xz != extrp;
			}
			const bool AtHeightmapExtrP(const int xz)
			{
				signed int extrn = static_cast<signed int>(m_corners.nn.x + 0.5f);
				signed int extrp = static_cast<signed int>(m_corners.pp.x - 0.5f);
				return xz == extrp && xz != extrn;
			}
			void NeighbouringBiomes(biome::biome_t* nb, const WVec2& chunkCoords, 
				terrain::Terrain& t, const WVec2& negCorner, 
				const signed int x, const signed int z, pnoise::PNoise::CellCorners& bcc, 
				pnoise::PNoise::GradientVectors& gv)
			{
				if (AtExtr0(x)) nb[0] = DetermineNeighbouringBiome(t, chunkExtr_t::NEG_X, chunkCoords, negCorner, x, z);
				else nb[0] = t.Biome(glm::vec2(negCorner.x + x - 1, negCorner.z + z), bcc, gv);

				if (AtExtr15(x)) nb[1] = DetermineNeighbouringBiome(t, chunkExtr_t::POS_X, chunkCoords, negCorner, x, z);
				else nb[1] = t.Biome(glm::vec2(negCorner.x + x + 1, negCorner.z + z), bcc, gv);

				if (AtExtr15(z)) nb[2] = DetermineNeighbouringBiome(t, chunkExtr_t::POS_Z, chunkCoords, negCorner, x, z);
				else nb[2] = t.Biome(glm::vec2(negCorner.x + x, negCorner.z + z + 1), bcc, gv);
				
				if (AtExtr0(z)) nb[3] = DetermineNeighbouringBiome(t, chunkExtr_t::NEG_Z, chunkCoords, negCorner, x, z);
				else nb[3] = t.Biome(glm::vec2(negCorner.x + x, negCorner.z + z - 1), bcc, gv);
			}
			pnoise::PNoise::CellCorners NeighbouringHeightmapCellCorners(terrain::Terrain& t, WVec2& neighbouringchunkcoord)
			{
				return t.CellCorners(neighbouringchunkcoord, terrain::Terrain::choice_t::HM);
			}
			pnoise::PNoise::GradientVectors NeighbouringHeightmapCellGVectors(pnoise::PNoise::CellCorners c, terrain::Terrain& t)
			{
				return t.GVectors(c, terrain::Terrain::choice_t::HM);
			}
			signed int DetermineNeighbouringHeight(biome::biome_t* nb, terrain::Terrain& t, 
				const chunkExtr_t ce, const WVec2& negCorner, signed int x, signed int z, WVec2& chunkCoord)
			{
				static float offsetx[4]
				{
					-16.0f, 16.0f, 0.0f, 0.0f
				};
				static float offsetz[4]
				{
					0.0f, 0.0f, 16.0f, -16.0f
				};

				glm::vec2 offsetv = glm::vec2(offsetx[static_cast<signed int>(ce)], offsetz[static_cast<signed int>(ce)]);

				signed int xOnOtherChunk = (static_cast<signed int>(ce) <= 1) ? ((ce == chunkExtr_t::NEG_X) ? 15 : 0) : x;
				signed int zOnOtherChunk = (static_cast<signed int>(ce) >= 2) ? ((ce == chunkExtr_t::NEG_Z) ? 15 : 0) : z;

				WVec2 blockC = { 
					negCorner.x +
					static_cast<signed int>(offsetx[static_cast<signed int>(ce)]), 
					negCorner.z +
					static_cast<signed int>(offsetz[static_cast<signed int>(ce)]) };

				signed int biomeMaxHeight = t.BiomeMaxHeight(nb[static_cast<signed int>(ce)]);
				signed int biomeOffset = t.BiomeOffset(nb[static_cast<signed int>(ce)]);

				pnoise::PNoise::GradientVectors gv;
				pnoise::PNoise::CellCorners cc;

				bool atCornerOfHMCell = false;

				if (ce == chunkExtr_t::NEG_X)
				{
					if (AtHeightmapExtrN(x + negCorner.x))
					{
						cc = t.CellCorners({ chunkCoord.x - 1, chunkCoord.z }, terrain::Terrain::choice_t::HM);
						gv = t.GVectors(cc, terrain::Terrain::choice_t::HM);
						atCornerOfHMCell = true;
					}
				}
				else if (ce == chunkExtr_t::POS_X)
				{
					if (AtHeightmapExtrP(x + negCorner.x))
					{
						cc = t.CellCorners({ chunkCoord.x + 1, chunkCoord.z }, terrain::Terrain::choice_t::HM);
						gv = t.GVectors(cc, terrain::Terrain::choice_t::HM);
						atCornerOfHMCell = true;
					}
				}
				else if (ce == chunkExtr_t::POS_Z)
				{
					if (AtHeightmapExtrP(z + negCorner.z))
					{
						cc = t.CellCorners({ chunkCoord.x, chunkCoord.z + 1 }, terrain::Terrain::choice_t::HM);
						gv = t.GVectors(cc, terrain::Terrain::choice_t::HM);
						atCornerOfHMCell = true;
					}
				}
				else if (ce == chunkExtr_t::NEG_Z)
				{
					if (AtHeightmapExtrN(z + negCorner.z))
					{
						cc = t.CellCorners({ chunkCoord.x, chunkCoord.z - 1}, terrain::Terrain::choice_t::HM);
						gv = t.GVectors(cc, terrain::Terrain::choice_t::HM);
						atCornerOfHMCell = true;
					}
				}
				if (!atCornerOfHMCell)
				{
					gv = m_gradientVectors;
					cc = m_corners;
				}

				return Height(blockC, 
					xOnOtherChunk, zOnOtherChunk,
					biomeMaxHeight, cc, gv, t) + 
					biomeOffset;
			}
			void NeighbouringHeights(biome::biome_t* nb, terrain::Terrain& t, const WVec2& negCorner, 
				signed int* nh, signed int x, signed int z, WVec2& chunkCoord)
			{
				if (AtExtr0(x)) nh[0] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::NEG_X, negCorner, x, z, chunkCoord);
				else nh[0] = Height(negCorner, x - 1, z, t.BiomeMaxHeight(nb[0]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[0]);

				if (AtExtr15(x)) nh[1] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::POS_X, negCorner, x, z, chunkCoord);
				else nh[1] = Height(negCorner, x + 1, z, t.BiomeMaxHeight(nb[1]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[1]);

				if (AtExtr15(z)) nh[2] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::POS_Z, negCorner, x, z, chunkCoord);
				else nh[2] = Height(negCorner, x, z + 1, t.BiomeMaxHeight(nb[2]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[2]);

				if (AtExtr0(z)) nh[3] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::NEG_Z, negCorner, x, z, chunkCoord);
				else nh[3] = Height(negCorner, x, z - 1, t.BiomeMaxHeight(nb[3]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[3]);
			}
			signed int SmallestNeighbour(signed int* nh)
			{
				unsigned int smallest = 0;
				for (unsigned int i = 1; i < 4; ++i)
				{
					if (nh[smallest] > nh[i])
						smallest = i;
				}
				return nh[smallest];
			}
			void AppendBlock(signed int x, signed int z, signed int y, 
				terrain::Terrain& t, biome::biome_t b, signed int bysH, BlockYStrip& bys)
			{
				CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
				bys.ystrip[y] = Block(CompressChunkCoord(cc), t.BlockType(b, bysH, y));
			}
			void GenerateHeightmapCellCorners(const WVec2& chunkCoord, terrain::Terrain& t)
			{
				m_corners = t.CellCorners(chunkCoord, terrain::Terrain::choice_t::HM);
			}
			void GenerateHeightmapCellGVectors(terrain::Terrain& t)
			{
				m_gradientVectors = t.GVectors(m_corners, terrain::Terrain::choice_t::HM);
			}
		private:
			gpu::CGPUHandler m_gpuh;
			// heightmap cell corners
			pnoise::PNoise::CellCorners m_corners;
			// chunk corners
			CCorners m_ccorners;
			// heightmap cell gradient vectors
			pnoise::PNoise::GradientVectors m_gradientVectors;
			BlockYStrip m_blocks[16 * 16];
		};
	}
}
#endif