#ifndef CHUNK_DATA_BASE_HEADER
#define CHUNK_DATA_BASE_HEADER

#include <unordered_map>

#include "../biome/biomemap/biomemap.h"
#include "../../block/block.h"
#include "../gpu/gpuhandler/chunk_gpu_side_handler.h"
#include "../cdata/cvec2.h"
#include "../noise/regular/reg_perlin_noise.h"
#include "../biome/biome_handler.h"
#include "blockystrip.h"
#include "../biome/plains/plains.h"

namespace minecraft
{
	namespace chunk
	{
		class ChunkDB
		{
		public:
			explicit ChunkDB(signed int seed)
				: m_perlinNoiseGenerator(new noise::Reg_PerlinNoise(seed))
			{
			}
		public:
			void Load(WVec2 chunkCoords, WVec2 negCorner, biome::BiomeHandler& bh)
			{
				biome::BiomeMap::BCCorners bcc = bh.BiomeMapCellCorners(chunkCoords);
				biome::BiomeMap::GradientVectors gv = bh.GVectors(bcc);

				GenerateCorners(negCorner);
				m_gradientVectors = m_perlinNoiseGenerator->GVectors(m_corners);
				for (signed int z = 0; z < 16; ++z)
				{
					for (signed int x = 0; x < 16; ++x)
					{
						biome::BiomeMap::DifferenceVectors dv = bh.DVectors(glm::vec2(negCorner.x + x, negCorner.z + z), bcc);
						biome::biome_t b = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x, negCorner.z + z)), bcc, gv);
						float height = Height(negCorner, x, z, bh.MaxBiomeHeight(b), m_corners, m_gradientVectors) + 20;
						for (signed int y = -30; y < static_cast<signed int>(height); ++y)
						{
							CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
							BlockYStrip& bys = m_blocks[Index(cc)];
							bys.ystrip[y] = Block(CompressChunkCoord(cc), bh.BlockType(b, bh.MaxBiomeHeight(b), y));
						}
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
			void LoadTop(WVec2 chunkCoords, WVec2 negCorner, biome::BiomeHandler& bh)
			{
				biome::BiomeMap::BCCorners bcc = bh.BiomeMapCellCorners(chunkCoords);
 				biome::BiomeMap::GradientVectors gv = bh.GVectors(bcc);

				GenerateCorners(negCorner);
				m_gradientVectors = m_perlinNoiseGenerator->GVectors(m_corners);

				for (signed int z = 0; z < 16; ++z)
				{
					for (signed int x = 0; x < 16; ++x)
					{
						// biome of the current block
						biome::biome_t b = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x, negCorner.z + z)), bcc, gv);

						// calculating neibouring biomes
						biome::biome_t neighbouringBiomes[4];
						NeighbouringBiomes(neighbouringBiomes, chunkCoords, bh, negCorner, x, z, bcc, gv);

						float neighbouringHeights[4];
						NeighbouringHeights(neighbouringBiomes, bh, negCorner, neighbouringHeights, x, z);

						// height of current block
						signed int h = static_cast<signed int>(Height(negCorner, x, z, 
							bh.MaxBiomeHeight(b), m_corners, m_gradientVectors) + bh.BiomeOffset(b));

						// calculating smallest height
						signed int smallestNeighbour = SmallestNeighbour(neighbouringHeights);

						// regardless of whether at corner of chunk
						for (signed int y = h; y >= smallestNeighbour; --y) AppendBlock(x, z, y, bh, b, h);
						if (h < smallestNeighbour) AppendBlock(x, z, h, bh, b, h);
					}
				}
				LoadGPUData(chunkCoords, negCorner);
			}
			void LoadGPUBuffer(void)
			{
				m_gpuh.LoadGPUBuffer();
			}
		public:
			/* getter */
			glm::vec3 WCoord(WVec2 wchunkCoord, CVec2 ccoord, unsigned char elevation, const WVec2&& negativeCornerWPos)
			{
				return m_blocks[Index(ccoord)].ystrip[elevation].WPos(wchunkCoord, elevation, negativeCornerWPos);
			}
			const Block::block_t BlockType(CVec2 ccoord, unsigned char elevation)
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
		private:
			void LoadGPUData(WVec2 chunkCoords, WVec2 negCorner)
			{
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

				/* using the negative corner of the chunk */
				/* the program calculates the positions of the corners of the chunk */
				m_corners.nn = glm::vec2(negativeCorner.x - 0.5f,
					negativeCorner.y - 0.5f);
				m_corners.np = glm::vec2(negativeCorner.x - 0.5f,
					negativeCorner.y + 15.5f);
				m_corners.pn = glm::vec2(negativeCorner.x + 15.5f,
					negativeCorner.y - 0.5f);
				m_corners.pp = glm::vec2(negativeCorner.x + 15.5f,
					negativeCorner.y + 15.5f);
			}
			float Height(const WVec2& negCorner, const signed int& x, const signed int& z, 
				signed int mh, CCorners& cc, noise::PerlinNoise::GradientVectors& gv)
			{
				float blockx = static_cast<float>(negCorner.x + x);
				float blockz = static_cast<float>(negCorner.z + z);
				glm::vec2 blockxzworld = glm::vec2(blockx, blockz);
				return m_perlinNoiseGenerator->Height(blockxzworld, cc, gv, mh);
			}

			// for the LoadTop() for the neighbouring biomes
			biome::biome_t DetermineNeighbouringBiome(biome::BiomeHandler& bh, 
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

				biome::BiomeMap::BCCorners bccnx = bh.BiomeMapCellCorners(WVec2{ 
					chunkCoords.x + offsetx[static_cast<unsigned int>(chunkExtr)], 
					chunkCoords.z + offsetz[static_cast<unsigned int>(chunkExtr)] });
				biome::BiomeMap::GradientVectors gvnx = bh.GVectors(bccnx);
				return bh.Biome(glm::vec2(negCorner.x + x + offsetx[static_cast<unsigned int>(chunkExtr)], 
					negCorner.z + z + offsetz[static_cast<unsigned int>(chunkExtr)]), bccnx, gvnx);
			}
			const bool AtExtr0(signed int xz)
			{
				return xz == 0 && xz != 15;
			}
			const bool AtExtr15(signed int xz)
			{
				return xz == 15 && xz != 0;
			}
			void NeighbouringBiomes(biome::biome_t* nb, const WVec2& chunkCoords, 
				biome::BiomeHandler& bh, const WVec2& negCorner, 
				const signed int x, const signed int z, biome::BiomeMap::BCCorners& bcc, biome::BiomeMap::GradientVectors& gv)
			{
				if (AtExtr0(x))nb[0] = DetermineNeighbouringBiome(bh, chunkExtr_t::NEG_X, chunkCoords, negCorner, x, z);
				else nb[0] = bh.Biome(glm::vec2(negCorner.x + x - 1, negCorner.z + z), bcc, gv);

				if (AtExtr15(x)) nb[1] = DetermineNeighbouringBiome(bh, chunkExtr_t::POS_X, chunkCoords, negCorner, x, z);
				else nb[1] = bh.Biome(glm::vec2(negCorner.x + x + 1, negCorner.z + z), bcc, gv);

				if (AtExtr15(z)) nb[2] = DetermineNeighbouringBiome(bh, chunkExtr_t::POS_Z, chunkCoords, negCorner, x, z);
				else nb[2] = bh.Biome(glm::vec2(negCorner.x + x, negCorner.z + z + 1), bcc, gv);
				
				if (AtExtr0(z)) nb[3] = DetermineNeighbouringBiome(bh, chunkExtr_t::NEG_Z, chunkCoords, negCorner, x, z);
				else nb[3] = bh.Biome(glm::vec2(negCorner.x + x, negCorner.z + z - 1), bcc, gv);
			}
			float DetermineNeighbouringHeight(biome::biome_t* nb, biome::BiomeHandler& bh, 
				const chunkExtr_t ce, const WVec2& negCorner, signed int x, signed int z)
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

				CCorners cc = { m_corners.nn + offsetv,
					m_corners.np + offsetv,
					m_corners.pn + offsetv,
					m_corners.pp + offsetv };

				signed int xOnOtherChunk = (static_cast<signed int>(ce) <= 1) ? ((ce == chunkExtr_t::NEG_X) ? 15 : 0) : x;
				signed int zOnOtherChunk = (static_cast<signed int>(ce) >= 2) ? ((ce == chunkExtr_t::NEG_Z) ? 15 : 0) : z;

				WVec2 blockC = { 
					negCorner.x +
					static_cast<signed int>(offsetx[static_cast<signed int>(ce)]), 
					negCorner.z +
					static_cast<signed int>(offsetz[static_cast<signed int>(ce)]) };

				signed int biomeMaxHeight = bh.MaxBiomeHeight(nb[static_cast<signed int>(ce)]);
				signed int biomeOffset = bh.BiomeOffset(nb[static_cast<signed int>(ce)]);

				noise::PerlinNoise::GradientVectors gv = m_perlinNoiseGenerator->GVectors(cc);
				return Height(blockC, 
					xOnOtherChunk, zOnOtherChunk,
					biomeMaxHeight, cc, gv) + 
					biomeOffset;
			}
			void NeighbouringHeights(biome::biome_t* nb, biome::BiomeHandler& bh, const WVec2& negCorner, float* nh, signed int x, signed int z)
			{
				if (AtExtr0(x)) nh[0] = DetermineNeighbouringHeight(nb, bh, chunkExtr_t::NEG_X, negCorner, x, z);
				else nh[0] = Height(negCorner, x - 1, z, bh.MaxBiomeHeight(nb[0]), m_corners, m_gradientVectors) + bh.BiomeOffset(nb[0]);

				if (AtExtr15(x)) nh[1] = DetermineNeighbouringHeight(nb, bh, chunkExtr_t::POS_X, negCorner, x, z);
				else nh[1] = Height(negCorner, x + 1, z, bh.MaxBiomeHeight(nb[1]), m_corners, m_gradientVectors) + bh.BiomeOffset(nb[1]);

				if (AtExtr15(z)) nh[2] = DetermineNeighbouringHeight(nb, bh, chunkExtr_t::POS_Z, negCorner, x, z);
				else nh[2] = Height(negCorner, x, z + 1, bh.MaxBiomeHeight(nb[2]), m_corners, m_gradientVectors) + bh.BiomeOffset(nb[2]);

				if (AtExtr0(z)) nh[3] = DetermineNeighbouringHeight(nb, bh, chunkExtr_t::NEG_Z, negCorner, x, z);
				else nh[3] = Height(negCorner, x, z - 1, bh.MaxBiomeHeight(nb[3]), m_corners, m_gradientVectors) + bh.BiomeOffset(nb[3]);
			}
			signed int SmallestNeighbour(float* nh)
			{
				unsigned int smallest = 0;
				for (unsigned int i = 1; i < 4; ++i)
				{
					if (nh[smallest] > nh[i])
						smallest = i;
				}
				return static_cast<signed int>(nh[smallest]);
			}
			void AppendBlock(signed int x, signed int z, signed int y, 
				biome::BiomeHandler& bh, biome::biome_t b, signed int bysH)
			{
				CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
				BlockYStrip& bys = m_blocks[Index(cc)];
				bys.ystrip[y] = Block(CompressChunkCoord(cc), bh.BlockType(b, bysH, y));
			}
		private:
			CCorners m_corners;
			gpu::CGPUHandler m_gpuh;
			noise::PerlinNoise::GradientVectors m_gradientVectors;
			noise::PerlinNoise* m_perlinNoiseGenerator;
			BlockYStrip m_blocks[16 * 16];
		};
	}
}
#endif