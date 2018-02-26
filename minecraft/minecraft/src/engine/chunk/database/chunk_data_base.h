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
				: m_numBlocks(0)
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
				pnoise::PNoise::GradientVectors gv = t.GVectors(bcc, terrain::Terrain::choice_t::BM);

				GenerateHeightmapCellCorners(chunkCoords, t);
				GenerateHeightmapCellGVectors(t);

				for (int32_t z = 0; z < 16; ++z)
				{
					for (int32_t x = 0; x < 16; ++x)
					{
						// load every blockc 
					}
				}
				LoadGPUData(chunkCoords, negCorner);
			}
			// chunk mesh
			enum class chunkExtr_t
				: uint32_t
			{
				NEG_X = 0,

				POS_X = 1,

				POS_Z = 2,

				NEG_Z = 3
			};
			void LoadStructures(WVec2 chunkCoords, structures::StructuresHandler& sh, WVec2 negCorner, terrain::Terrain& t)
			{
				auto discardSCBYS = [](){};

				WCoordChunk wcc = {chunkCoords};
				std::vector<structures::Structure*> structs = sh.AllStructuresOfChunk(wcc, negCorner, t);
				for (auto& i : structs)
				{
					if (i != nullptr)
					{
						structures::GenRange gr = i->GRange();
						WVec2 wpos = i->OriginW();

						for (int32_t z = wpos.z + gr.nz; z <= wpos.z + gr.pz; ++z)
						{
							for (int32_t x = wpos.x + gr.nx; x <= wpos.x + gr.px; ++x)
							{
								CVec2 cc = sh.SCompCoordInChunk(chunkCoords, { x , z});
								if (cc.x > 15 || cc.z > 15) 
									discardSCBYS();
								else
								{
									uint8_t index = Index(cc);
									BlockYStrip& bys = m_blocks[index];
									int32_t oheight = i->Height(); // function that returns the height of the origin

									structures::StructCompBYS scbys = i->GenerateYStripOfStruct({ x, z });
									int32_t start = scbys.start;
									int32_t end = scbys.end;

									for (int32_t y = start; y <= end; ++y)
									{
										AppendBlock(cc.x, cc.z, y + oheight, bys, scbys.bys[y].block);
										GradLoadGPUData(chunkCoords, negCorner, cc.x, cc.z, bys, y + oheight);
									}
								}
							}
						}
					}
				}
			}
			void LoadTop(WVec2 chunkCoords, WVec2 negCorner, terrain::Terrain& t)
			{
				GenerateCorners(negCorner);

				pnoise::PNoise::CellCorners bcc = t.CellCorners(chunkCoords, terrain::Terrain::choice_t::BM);
				pnoise::PNoise::GradientVectors gv = t.GVectors(bcc, terrain::Terrain::choice_t::BM);

				GenerateHeightmapCellCorners(chunkCoords, t);
				GenerateHeightmapCellGVectors(t);

				for (uint8_t z = 0; z < 16; ++z)
				{
					for (uint8_t x = 0; x < 16; ++x)
					{
						// biome of the current block
						glm::vec2 blockWCoord = glm::vec2(negCorner.x + x, negCorner.z + z);
						biome::biome_t b = t.Biome(blockWCoord, bcc, gv);

						// calculating neibouring biomes
						biome::biome_t neighbouringBiomes[4];
						NeighbouringBiomes(neighbouringBiomes, chunkCoords, t, negCorner, x, z, bcc, gv);

						int32_t neighbouringHeights[4];
						NeighbouringHeights(neighbouringBiomes, t, negCorner, neighbouringHeights, x, z, chunkCoords);

						// height of current block
						int32_t h = Height(negCorner, x, z, t.BiomeMaxHeight(b), m_corners, m_gradientVectors, t) + t.BiomeOffset(b);

						// calculating smallest height
						int32_t smallestNeighbour = SmallestNeighbour(neighbouringHeights);

						BlockYStrip& bys = m_blocks[(Index({ x, z }))];
						bys.bio = b;
						// regardless of whether at corner of chunk

						for (int32_t y = h; y >= smallestNeighbour; --y)
						{
							AppendBlock(x, z, y, t, b, h, bys);
							GradLoadGPUData(chunkCoords, negCorner, x, z, bys, y);
						}
						bys.smallest = smallestNeighbour;
						bys.top = h;
						if (h < smallestNeighbour)
						{
							AppendBlock(x, z, h, t, b, h, bys);
							GradLoadGPUData(chunkCoords, negCorner, x, z, bys, h);
							bys.smallest = h;
							bys.top = h;
						}

						// for oceans
						if (b == biome::biome_t::OCEAN)
						{
							int32_t height = t.BiomeOffset(biome::biome_t::DESERT) - 2;
							AppendBlock(x, z, height, bys, Block::block_t::WATER);
							GradLoadGPUData(chunkCoords, negCorner, x, z, bys, height);
						}
					}
				}
			}
			// data for checking if neighbouring chunks need to load blocks
			struct DBNeighbourChunkData {
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
			DBNeighbourChunkData DestroyBlock(CVec2 bcoord, float y, terrain::Terrain& t, WVec2 negCorner, WCoordChunk wcc)
			{
				DBNeighbourChunkData cbncd;
				cbncd.quant = 0;
				int32_t cast = static_cast<int32_t>(y);
				// erases y from the ystrip
				uint16_t vindex = m_blocks[Index(bcoord)].ystrip[cast].VIndex();
				m_blocks[Index(bcoord)].ystrip.at(cast).Valid() = false;
				m_gpuh.RemoveBlock(vindex);

				// check for neighbouring blocks
				LoadAllNeighbouringVisibleBlocks(bcoord, cast, t, negCorner, wcc, cbncd);

				if (m_gpuh.MaxDBPointer() == m_gpuh.DBPointer())
				{
					m_gpuh.DBPointer() = 0;
					UpdateIndices(m_gpuh.DeletedBlocksIndices());
					m_numBlocks -= 16;
				}
				return cbncd;
			}
			void LoadGPUBuffer(void)
			{
				m_gpuh.LoadGPUBuffer();
			}
			void DestroyHEAPMemoryForBlocksWPos(void)
			{
				m_gpuh.DestroyVector();
			}
			void LoadVisibleBlock(CVec2 c, int32_t y,
				terrain::Terrain& t, WVec2 negCorner, WVec2& wcc)
			{
				BlockYStrip& bys = m_blocks[Index(c)];
				uint8_t index = Index(c);
				if (BlockIsVisible(c.x, y, c.z, bys) &&
					m_blocks[index].ystrip.find(y) == m_blocks[index].ystrip.end())
				{
					if (y < bys.top)
					{
						Block::block_t b = t.BlockType(bys.bio, bys.top, y);
						Block& newB = AppendBlock(c.x, c.z, y, bys, b);
						newB.VIndex() = m_numBlocks - 1;
						m_gpuh.PushBack(newB.WPos(wcc, y, negCorner), newB.TextureD());
					}
				}
			}
		public:
			void LoadAllNeighbouringVisibleBlocks(CVec2 c, int32_t y, terrain::Terrain& t, 
				WVec2 negCorner, WCoordChunk& wcc, DBNeighbourChunkData& d)
			{
				// top
				BlockYStrip& byscenter = m_blocks[Index(c)];
				LoadNeighbouringVisibleBlock(c, y + 1, byscenter, t, negCorner, wcc);
				LoadNeighbouringVisibleBlock(c, y - 1, byscenter, t, negCorner, wcc);

				CVec2 xp = { c.x + 1u, c.z };
				if (xp.x == 16u) 
					d.App(wcc, c, y, {1, 0});
				else
				{
					BlockYStrip& bysxp = m_blocks[Index(xp)];
					LoadNeighbouringVisibleBlock(xp, y, bysxp, t, negCorner, wcc);
				}

				CVec2 xn = { c.x - 1u, c.z };
				if (xn.x == static_cast<uint8_t>(-1)) d.App(wcc, c, y, {-1, 0});
				else
				{
					BlockYStrip& bysxn = m_blocks[Index(xn)];
					LoadNeighbouringVisibleBlock(xn, y, bysxn, t, negCorner, wcc);
				}

				CVec2 zp = { c.x, c.z + 1u };
				if (zp.z == 16u) d.App(wcc, c, y, {0, 1});
				else
				{
					BlockYStrip& byszp = m_blocks[Index(zp)];
					LoadNeighbouringVisibleBlock(zp, y, byszp, t, negCorner, wcc);
				}

				CVec2 zn = { c.x, c.z - 1u };
				if (zn.z == static_cast<uint8_t>(-1)) d.App(wcc, c, y, {0, -1});
				else
				{
					BlockYStrip& byszn = m_blocks[Index(zn)];
					LoadNeighbouringVisibleBlock(zn, y, byszn, t, negCorner, wcc);
				}
			}
			void LoadNeighbouringVisibleBlock(CVec2 c, int32_t y, BlockYStrip& bys, 
				terrain::Terrain& t, WVec2 negCorner, WCoordChunk& wcc)
			{
				uint8_t index = Index(c);
				if (BlockIsVisible(c.x, y, c.z, bys) && 
					m_blocks[index].ystrip.find(y) == m_blocks[index].ystrip.end())
				{
					if (y < bys.top)
					{
						Block::block_t b = t.BlockType(bys.bio, bys.top, y);
						Block& newB = AppendBlock(c.x, c.z, y, bys, b);
						newB.VIndex() = m_numBlocks - 1;
						m_gpuh.PushBack(newB.WPos(wcc.wpos, y, negCorner), newB.TextureD());
					}
				}
			}
			
			/* getter */
			const bool BlockExists(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos)
			{
				int32_t y = static_cast<int32_t>(wpos.y);
				uint8_t index = Index(ccoord);
				// if not equal then the block exists
				if (m_blocks[index].ystrip.find(y) != m_blocks[index].ystrip.end())
				{
					return m_blocks[index].ystrip.at(y).Valid();
				}
				return false;
			}
			int32_t HightestBlock(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos)
			{
				int32_t smallest = 0xfffff;
				for (auto& bys : m_blocks[Index(ccoord)].ystrip)
					if (smallest > bys.first) smallest = bys.first;
				return smallest;
			}
			int32_t BlockUnder(WVec2 chunkCoord, CVec2 ccoord, glm::vec3 wpos, const WVec2& negativeCornerWPos)
			{
				int32_t y = static_cast<int32_t>(wpos.y + 1.0f);
				uint8_t index = Index(ccoord);
				auto& bys = m_blocks[index];
				for (; y >= bys.smallest; --y)
					if (bys.ystrip.find(y) != bys.ystrip.end()) return y;
				/* if there aren't any blocks under the player, function will return -0xff */
				return -0xff;
			}
			glm::vec3 WCoord(WVec2 wchunkCoord, CVec2 ccoord, int32_t elevation, const WVec2&& negativeCornerWPos)
			{
				return m_blocks[Index(ccoord)].ystrip[elevation].WPos(wchunkCoord, elevation, negativeCornerWPos);
			}
			const Block::block_t BlockType(CVec2 ccoord, int32_t elevation)
			{
				return m_blocks[Index(ccoord)].ystrip[elevation].BlockType();
			}
			uint8_t Index(CVec2 ccoord) const
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
			bool Destroyed(CVec2 c, int32_t y)
			{
				float yf = static_cast<float>(y);
				// top
				if (BlockExists(WVec2(), c, glm::vec3(0.0f, yf + 1.0f, 0.0f))) return true;
				// bottom
				if (BlockExists(WVec2(), c, glm::vec3(0.0f, yf - 1.0f, 0.0f))) return true;
				// xp
				if (BlockExists(WVec2(), {c.x + 1u, c.z}, glm::vec3(0.0f, yf, 0.0f))) return true;
				// xn
				if (BlockExists(WVec2(), { c.x - 1u, c.z }, glm::vec3(0.0f, yf, 0.0f))) return true;
				// zp
				if (BlockExists(WVec2(), { c.x, c.z + 1u }, glm::vec3(0.0f, yf, 0.0f))) return true;
				// zn
				if (BlockExists(WVec2(), { c.x, c.z - 1u }, glm::vec3(0.0f, yf, 0.0f))) return true;
			}
			void UpdateIndices(std::array<uint32_t, 16>& indices)
			{
				std::sort(indices.begin(), indices.end());
				for (uint32_t i = 0; i < 16 * 16; ++i)
				{
					BlockYStrip& bys = m_blocks[i];
					for (auto& j : bys.ystrip)
					{
						uint16_t& index = j.second.VIndex();
						uint16_t original = index;
						for (uint32_t indexDB : indices)
						{
							if (indexDB < original)
							{
								--index;
							}
							else break;
						}
					}
				}
			}
			void LoadGPUData(WVec2 chunkCoords, WVec2 negCorner)
			{
				m_gpuh.PrepareVector();
				for (uint8_t z = 0; z < 16; ++z)
				{
					for (uint8_t x = 0; x < 16; ++x)
					{
						CVec2 cc = { x, z };
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
				for (uint8_t z = 0; z < 16; ++z)
				{
					for (uint8_t x = 0; x < 16; ++x)
					{
						CVec2 cc = { x, z };
						BlockYStrip& bys = m_blocks[Index(cc)];
						for (auto& i : bys.ystrip)
						{
							m_gpuh.Load(&bys, Index(cc), i.first, chunkCoords, negCorner);
						}
					}
				}
			}
			void GradLoadGPUData(WVec2 chunkCoord, WVec2 negCorner, uint8_t x, uint8_t z, BlockYStrip& bys, int32_t y)
			{
				CVec2 cc = { x, z };
				m_gpuh.Load(&bys, Index(cc), y, chunkCoord, negCorner);
			}
			bool BlockIsVisible(uint8_t x, int32_t y, uint8_t z, const BlockYStrip& bys)
			{
				// is at the edge of the chunk
				if (x == 15 || z == 15 || x == 0 || z == 0) return true;
				// check x axis
				CVec2 cc = { x + 1u, z };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;
				cc = { x - 1u, z };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;

				cc = { x, z + 1u };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;
				cc = { x, z - 1u };
				if (m_blocks[Index(cc)].ystrip.find(y) == m_blocks[Index(cc)].ystrip.end()) return true;

				// check y axis
				cc = { x, z };
				if (m_blocks[Index(cc)].ystrip.find(y + 1) == m_blocks[Index(cc)].ystrip.end()) return true;
				if (m_blocks[Index(cc)].ystrip.find(y - 1) == m_blocks[Index(cc)].ystrip.end()) return true;

				else return false;
			}
			CCoord CompressChunkCoord(CVec2 cc) const
			{
				uint8_t x = cc.x;
				uint8_t z = cc.z;
				uint8_t res = (x << 4) + z;
				return { res };
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
			int32_t Height(const WVec2& negCorner, const uint8_t& x, const uint8_t& z, 
				int32_t mh, pnoise::PNoise::CellCorners& cc, pnoise::PNoise::GradientVectors& gv, terrain::Terrain& t) 
			{
				float blockx = static_cast<float>(negCorner.x + x);
				float blockz = static_cast<float>(negCorner.z + z);
				glm::vec2 blockxzworld = glm::vec2(blockx, blockz);
				return t.Height(blockxzworld, cc, gv, mh);
			}

			// for the LoadTop() for the neighbouring biomes
			biome::biome_t DetermineNeighbouringBiome(terrain::Terrain& t, 
				const chunkExtr_t chunkExtr, const WVec2& chunkCoords, const WVec2& negCorner, uint8_t x, uint8_t z)
			{
				static int32_t offsetx[4]
				{
					-1, 1, 0, 0
				};
				static int32_t offsetz[4]
				{
					0, 0, 1, -1
				};

				uint32_t chunkExtrUint32Cast = static_cast<uint32_t>(chunkExtr);
				pnoise::PNoise::CellCorners bccnx = t.CellCorners(WVec2{
					chunkCoords.x + offsetx[chunkExtrUint32Cast],			// static cast because of enum class
					chunkCoords.z + offsetz[chunkExtrUint32Cast] }, terrain::Terrain::choice_t::BM);
				pnoise::PNoise::GradientVectors gvnx = t.GVectors(bccnx, terrain::Terrain::choice_t::BM);
				return t.Biome(glm::vec2(negCorner.x + x + offsetx[chunkExtrUint32Cast],
					negCorner.z + z + offsetz[chunkExtrUint32Cast]), bccnx, gvnx);
			}
			const bool AtExtr0(uint8_t xzInChunk)
			{
				return xzInChunk == 0 && xzInChunk != 15;
			}
			const bool AtExtr15(uint8_t xzInChunk)
			{
				return xzInChunk == 15 && xzInChunk != 0;
			}
			const bool AtHeightmapExtrN(const int32_t xzWorld)
			{
				int32_t extrn = static_cast<int32_t>(m_corners.nn.x + 0.5f);
				int32_t extrp = static_cast<int32_t>(m_corners.pp.x - 0.5f);
				return xzWorld == extrn && xzWorld != extrp;
			}
			const bool AtHeightmapExtrP(const int32_t xzWorld)
			{
				int32_t extrn = static_cast<int32_t>(m_corners.nn.x + 0.5f);
				int32_t extrp = static_cast<int32_t>(m_corners.pp.x - 0.5f);
				return xzWorld == extrp && xzWorld != extrn;
			}
			void NeighbouringBiomes(biome::biome_t* nb, const WVec2& chunkCoords, 
				terrain::Terrain& t, const WVec2& negCorner, 
				const uint8_t x, const uint8_t z, pnoise::PNoise::CellCorners& bcc, 
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
				const chunkExtr_t ce, const WVec2& negCorner, uint8_t xInChunk, uint8_t zInChunk, WVec2& chunkCoord)
			{
				static float offsetx[4]
				{
					-16.0f, 16.0f, 0.0f, 0.0f
				};
				static float offsetz[4]
				{
					0.0f, 0.0f, 16.0f, -16.0f
				};

				int32_t chunkExtrUInt32Cast = static_cast<uint32_t>(ce);
				glm::vec2 offsetv = glm::vec2(offsetx[chunkExtrUInt32Cast], offsetz[chunkExtrUInt32Cast]);

				uint8_t xOnOtherChunk = (chunkExtrUInt32Cast <= 1) ? ((ce == chunkExtr_t::NEG_X) ? 15 : 0) : xInChunk;
				uint8_t zOnOtherChunk = (chunkExtrUInt32Cast >= 2) ? ((ce == chunkExtr_t::NEG_Z) ? 15 : 0) : zInChunk;

				WVec2 blockC = { 
					negCorner.x +
					static_cast<int32_t>(offsetx[chunkExtrUInt32Cast]),
					negCorner.z +
					static_cast<int32_t>(offsetz[chunkExtrUInt32Cast]) };

				int32_t biomeMaxHeight = t.BiomeMaxHeight(nb[chunkExtrUInt32Cast]);
				int32_t biomeOffset = t.BiomeOffset(nb[chunkExtrUInt32Cast]);

				pnoise::PNoise::GradientVectors gv;
				pnoise::PNoise::CellCorners cc;

				bool atCornerOfHMCell = false;

				if (ce == chunkExtr_t::NEG_X)
				{
					if (AtHeightmapExtrN(xInChunk + negCorner.x))
					{
						cc = t.CellCorners({ chunkCoord.x - 1, chunkCoord.z }, terrain::Terrain::choice_t::HM);
						gv = t.GVectors(cc, terrain::Terrain::choice_t::HM);
						atCornerOfHMCell = true;
					}
				}
				else if (ce == chunkExtr_t::POS_X)
				{
					if (AtHeightmapExtrP(xInChunk + negCorner.x))
					{
						cc = t.CellCorners({ chunkCoord.x + 1, chunkCoord.z }, terrain::Terrain::choice_t::HM);
						gv = t.GVectors(cc, terrain::Terrain::choice_t::HM);
						atCornerOfHMCell = true;
					}
				}
				else if (ce == chunkExtr_t::POS_Z)
				{
					if (AtHeightmapExtrP(zInChunk + negCorner.z))
					{
						cc = t.CellCorners({ chunkCoord.x, chunkCoord.z + 1 }, terrain::Terrain::choice_t::HM);
						gv = t.GVectors(cc, terrain::Terrain::choice_t::HM);
						atCornerOfHMCell = true;
					}
				}
				else if (ce == chunkExtr_t::NEG_Z)
				{
					if (AtHeightmapExtrN(zInChunk + negCorner.z))
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
				int32_t* nh, uint8_t xInChunk, uint8_t zInChunk, WVec2& chunkCoord)
			{
				if (AtExtr0(xInChunk)) nh[0] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::NEG_X, negCorner, xInChunk, zInChunk, chunkCoord);
				else nh[0] = Height(negCorner, xInChunk - 1, zInChunk, t.BiomeMaxHeight(nb[0]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[0]);

				if (AtExtr15(xInChunk)) nh[1] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::POS_X, negCorner, xInChunk, zInChunk, chunkCoord);
				else nh[1] = Height(negCorner, xInChunk + 1, zInChunk, t.BiomeMaxHeight(nb[1]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[1]);

				if (AtExtr15(zInChunk)) nh[2] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::POS_Z, negCorner, xInChunk, zInChunk, chunkCoord);
				else nh[2] = Height(negCorner, xInChunk, zInChunk + 1, t.BiomeMaxHeight(nb[2]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[2]);

				if (AtExtr0(zInChunk)) nh[3] = DetermineNeighbouringHeight(nb, t, chunkExtr_t::NEG_Z, negCorner, xInChunk, zInChunk, chunkCoord);
				else nh[3] = Height(negCorner, xInChunk, zInChunk - 1, t.BiomeMaxHeight(nb[3]), m_corners, m_gradientVectors, t) + t.BiomeOffset(nb[3]);
			}
			int32_t SmallestNeighbour(int32_t* nh)
			{
				uint32_t smallest = 0;
				for (uint32_t i = 1; i < 4; ++i)
				{
					if (nh[smallest] > nh[i])
						smallest = i;
				}
				return nh[smallest];
			}
			void AppendBlock(uint8_t x, uint8_t z, int32_t y, 
				terrain::Terrain& t, biome::biome_t b, int32_t bysH, BlockYStrip& bys)
			{
				CVec2 cc = { x, z };
				bys.ystrip[y] = Block(CompressChunkCoord(cc), t.BlockType(b, bysH, y));
				++m_numBlocks;
			}
			Block& AppendBlock(uint8_t x, uint8_t z, int32_t y,
				BlockYStrip& bys, Block::block_t b)
			{
				CVec2 cc = { x, z };
				Block& newB = bys.ystrip[y];
				newB = Block(CompressChunkCoord(cc), b);
				++m_numBlocks;
				return newB;
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
			uint32_t m_numBlocks;
		};
	}
}
#endif