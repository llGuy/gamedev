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
			void LoadTop(WVec2 chunkCoords, WVec2 negCorner, biome::BiomeHandler& bh)
			{
				biome::BiomeMap::BCCorners bcc = bh.BiomeMapCellCorners(chunkCoords);
 				biome::BiomeMap::GradientVectors gv = bh.GVectors(bcc);

				// neighbouring chunk data
				// positive z
				biome::BiomeMap::BCCorners bccpz = bh.BiomeMapCellCorners(WVec2{ chunkCoords.x, chunkCoords.z + 1 });
				biome::BiomeMap::GradientVectors gvpz = bh.GVectors(bccpz);
				// negative z
				

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
						if (x == 0 && x != 15)
						{
							biome::BiomeMap::BCCorners bccnx = bh.BiomeMapCellCorners(WVec2{ chunkCoords.x - 1, chunkCoords.z });
							biome::BiomeMap::GradientVectors gvnx = bh.GVectors(bccnx);
							neighbouringBiomes[0] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x - 1, negCorner.z + z)), bccnx, gvnx);
						}
						else neighbouringBiomes[0] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x - 1, negCorner.z + z)), bcc, gv);
						if (x == 15 && x != 0)
						{
							biome::BiomeMap::BCCorners bccpx = bh.BiomeMapCellCorners(WVec2{ chunkCoords.x + 1, chunkCoords.z });
							biome::BiomeMap::GradientVectors gvpx = bh.GVectors(bccpx);
							neighbouringBiomes[1] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x + 1, negCorner.z + z)), bccpx, gvpx);
						}
						else neighbouringBiomes[1] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x + 1, negCorner.z + z)), bcc, gv);
						if (z == 15)
						{
							biome::BiomeMap::BCCorners bccpz = bh.BiomeMapCellCorners(WVec2{ chunkCoords.x, chunkCoords.z + 1 });
							biome::BiomeMap::GradientVectors gvpz = bh.GVectors(bccpz);
							neighbouringBiomes[2] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x, negCorner.z + z + 1)), bccpz, gvpz);
						}
						else neighbouringBiomes[2] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x, negCorner.z + z + 1)), bcc, gv);
						if (z == 0)
						{
							biome::BiomeMap::BCCorners bccnz = bh.BiomeMapCellCorners(WVec2{ chunkCoords.x, chunkCoords.z - 1 });
							biome::BiomeMap::GradientVectors gvnz = bh.GVectors(bccnz);
							neighbouringBiomes[3] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x, negCorner.z + z - 1)), bccnz, gvnz);
						}
						else neighbouringBiomes[3] = bh.Biome(glm::vec2(glm::vec2(negCorner.x + x, negCorner.z + z - 1)), bcc, gv);
						if (neighbouringBiomes[0] != b)
							std::cout << "hi" << std::endl;

						float neighbouringHeights[4];
						
						if (x == 0 && x != 15)
						{
							CCorners cc = { m_corners.nn - glm::vec2(16.0f, 0.0f), m_corners.np - glm::vec2(16.0f, 0.0f),
								m_corners.pn - glm::vec2(16.0f, 0.0f), m_corners.pp - glm::vec2(16.0f, 0.0f) };
							noise::PerlinNoise::GradientVectors gv = m_perlinNoiseGenerator->GVectors(cc);
							
							neighbouringHeights[0] = Height({ negCorner.x - 16, negCorner.z }, 15, z,
								bh.MaxBiomeHeight(neighbouringBiomes[0]), cc, gv) + bh.BiomeOffset(neighbouringBiomes[0]);
						}
						else neighbouringHeights[0] = Height(negCorner, x - 1, z, 
							bh.MaxBiomeHeight(neighbouringBiomes[0]), m_corners, m_gradientVectors) + bh.BiomeOffset(neighbouringBiomes[0]);
						if(x == 15 && x != 0)
						{
							CCorners cc = { m_corners.nn + glm::vec2(16.0f, 0.0f), m_corners.np + glm::vec2(16.0f, 0.0f),
								m_corners.pn + glm::vec2(16.0f, 0.0f), m_corners.pp + glm::vec2(16.0f, 0.0f) };
							noise::PerlinNoise::GradientVectors gv = m_perlinNoiseGenerator->GVectors(cc);

							neighbouringHeights[1] = Height({ negCorner.x + 16, negCorner.z }, 0, z,
								bh.MaxBiomeHeight(neighbouringBiomes[1]), cc, gv) + bh.BiomeOffset(neighbouringBiomes[1]);
						}
						else neighbouringHeights[1] = Height(negCorner, x + 1, z,
							bh.MaxBiomeHeight(neighbouringBiomes[1]), m_corners, m_gradientVectors) + bh.BiomeOffset(neighbouringBiomes[1]);
						if (z == 15 && z != 0)
						{
							CCorners cc = { m_corners.nn + glm::vec2(0.0f, 16.0f), m_corners.np + glm::vec2(0.0f, 16.0f),
								m_corners.pn + glm::vec2(0.0f, 16.0f), m_corners.pp + glm::vec2(0.0f, 16.0f) };
							noise::PerlinNoise::GradientVectors gv = m_perlinNoiseGenerator->GVectors(cc);

							neighbouringHeights[2] = Height({ negCorner.x, negCorner.z + 16 }, x, 0,
								bh.MaxBiomeHeight(neighbouringBiomes[2]), cc, gv) + bh.BiomeOffset(neighbouringBiomes[2]);
						}
						else neighbouringHeights[2] = Height(negCorner, x, z + 1,
							bh.MaxBiomeHeight(neighbouringBiomes[2]), m_corners, m_gradientVectors) + bh.BiomeOffset(neighbouringBiomes[2]);
						if (z == 0 && z != 15)
						{
							CCorners cc = { m_corners.nn - glm::vec2(0.0f, 16.0f), m_corners.np - glm::vec2(0.0f, 16.0f),
								m_corners.pn - glm::vec2(0.0f, 16.0f), m_corners.pp - glm::vec2(0.0f, 16.0f) };
							noise::PerlinNoise::GradientVectors gv = m_perlinNoiseGenerator->GVectors(cc);

							neighbouringHeights[3] = Height({ negCorner.x, negCorner.z - 16 }, x, 15,
								bh.MaxBiomeHeight(neighbouringBiomes[3]), cc, gv) + bh.BiomeOffset(neighbouringBiomes[3]);
						}
						else neighbouringHeights[3] = Height(negCorner, x, z - 1,
							bh.MaxBiomeHeight(neighbouringBiomes[3]), m_corners, m_gradientVectors) + bh.BiomeOffset(neighbouringBiomes[3]);

						// height of current block
						float height = Height(negCorner, x, z, bh.MaxBiomeHeight(b), m_corners, m_gradientVectors) + bh.BiomeOffset(b);
						signed int h = static_cast<signed int>(height);

						// calculating smallest height
						unsigned int smallest = 0;
						for (unsigned int i = 1; i < 4; ++i)
						{
							if (neighbouringHeights[smallest] > neighbouringHeights[i])
								smallest = i;
						}
						signed int smallestNeighbour = static_cast<signed int>(neighbouringHeights[smallest]);

						// regardless of whether at corner of chunk
						for (signed int y = h; y >= smallestNeighbour; --y)
						{
							CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
							BlockYStrip& bys = m_blocks[Index(cc)];
							bys.ystrip[y] = Block(CompressChunkCoord(cc), bh.BlockType(b, h, y));
						}
						if (h < smallestNeighbour)
						{
							CVec2 cc = { static_cast<unsigned char>(x), static_cast<unsigned char>(z) };
							BlockYStrip& bys = m_blocks[Index(cc)];
							bys.ystrip[h] = Block(CompressChunkCoord(cc), bh.BlockType(b, h, h));
						}
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
			const Block::BlType BlockType(CVec2 ccoord, unsigned char elevation)
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