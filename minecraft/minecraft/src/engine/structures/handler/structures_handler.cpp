#include "structures_handler.h"
#include <iostream>

namespace minecraft
{
	namespace structures
	{
		StructuresHandler::StructuresHandler(signed int seed)
			: m_seed(seed)
		{
		}
		// all structures including the partial ones
		std::vector<Structure*> StructuresHandler::AllStructuresOfChunk(chunk::WCoordChunk wcc,
			WVec2 negCorner, terrain::Terrain& t) 
		{
			std::vector<Structure*> allStructures;
			std::vector<Structure*> currentChunk = StructuresOfChunk(wcc, negCorner, t);
			allStructures.insert(allStructures.end(), currentChunk.begin(), currentChunk.end());
			
			// left
			for (signed int z = wcc.wpos.z - 1, x = wcc.wpos.x - 1; z <= wcc.wpos.z + 1; ++z)
			{
				std::vector<Structure*> n = StructuresOfNeighbouringChunks(wcc, { { x, z } }, { x * 16 - 8, z * 16 - 8 }, t);
				if (n.size() > 0) 
					allStructures.insert(allStructures.end(), n.begin(), n.end());
			}
			// right
			for (signed int z = wcc.wpos.z - 1, x = wcc.wpos.x + 1; z <= wcc.wpos.z + 1; ++z)
			{
				std::vector<Structure*> n = StructuresOfNeighbouringChunks(wcc, { { x, z } }, { x * 16 - 8, z * 16 - 8 }, t);
				if (n.size() > 0) 
					allStructures.insert(allStructures.end(), n.begin(), n.end());
			}
			// top
			std::vector<Structure*> nt = StructuresOfNeighbouringChunks(wcc, { { wcc.wpos.x, wcc.wpos.z + 1 } },
				{ wcc.wpos.x * 16 - 8, (wcc.wpos.z + 1) * 16 - 8 }, t);
			if (nt.size() > 0)
				allStructures.insert(allStructures.end(), nt.begin(), nt.end());

			// bottom
			std::vector<Structure*> nb = StructuresOfNeighbouringChunks(wcc, { { wcc.wpos.x, wcc.wpos.z - 1 } },
				{ wcc.wpos.x * 16 - 8, (wcc.wpos.z - 1) * 16 - 8 }, t);
			if (nb.size() > 0) 
				allStructures.insert(allStructures.end(), nb.begin(), nb.end());
	
			return allStructures;
		}
		std::vector<Structure*> StructuresHandler::StructuresOfChunk(chunk::WCoordChunk wcc, 
			WVec2 negCorner, terrain::Terrain& t) const
		{
			std::vector<Structure*> v;

			int32_t hcomp1 = wcc.wpos.x * 0x145;
			int32_t hcomp2 = wcc.wpos.z * 0xf1a;
			int32_t hash = std::hash<signed int>()(hcomp1 + hcomp2 + m_seed);
			srand(hash);
			unsigned int size = rand() % 3;
			v.resize(size);

			if (size != 0)
			{
				pnoise::PNoise::CellCorners cc = t.CellCorners(wcc.wpos, terrain::Terrain::choice_t::BM);
				pnoise::PNoise::GradientVectors gv = t.GVectors(cc, terrain::Terrain::choice_t::BM);

				pnoise::PNoise::CellCorners hcc = t.CellCorners(wcc.wpos, terrain::Terrain::choice_t::HM);
				pnoise::PNoise::GradientVectors hgv = t.GVectors(hcc, terrain::Terrain::choice_t::HM);

				for (unsigned int i = 0; i < size; ++i)
				{
					uint8_t x = abs(rand() % 16);
					uint8_t z = abs(rand() % 16);

					biome::biome_t bio = t.Biome(glm::vec2(negCorner.x + x, negCorner.z + z), cc, gv);

					int32_t height = t.Height(glm::vec2(negCorner.x + x, negCorner.z + z), hcc, hgv, t.BiomeMaxHeight(bio)) +
						t.BiomeOffset(bio);

					// for now there are only trees
					if (bio != biome::biome_t::DESERT)
					{
						v[i] = new Tree(wcc, { x, z }, { -2, 2, -2, 2 }, tsize_t::MEDIUM,
							{ negCorner.x + x, negCorner.z + z }, structure_t::TREE, height);
					}
				}
			}
			return v;
		}
		std::vector<Structure*> StructuresHandler::StructuresOfNeighbouringChunks(chunk::WCoordChunk wcc,
				chunk::WCoordChunk nwcc, WVec2 negCorner, terrain::Terrain& t) const
		{
			std::vector<Structure*> v = StructuresOfChunk(nwcc, negCorner, t);
			std::vector<Structure*> r; // result
			for (auto& s : v)
			{
				if (s != nullptr)
				{
					int32_t ox = s->OriginW().x;
					int32_t oz = s->OriginW().z;
					GenRange& gr = s->GRange();

					int32_t nx = -0xff, px = -0xff, nz = -0xff, pz = - 0xff;
					// negative x, positive x, ...
					bool success = false;

					// finding the negative x and negative z and positive x
					for (int32_t z = oz + gr.nz; z <= oz + gr.pz; ++z)
					{
						for (int32_t x = ox + gr.nx; x <= ox + gr.px; ++x)
						{
							chunk::WCoordChunk ccosc = ChunkCoordOfSComp({ x,z });
							if (ccosc.wpos.x == wcc.wpos.x && ccosc.wpos.z == wcc.wpos.z)
							{
								if (nx == -0xff && nz == -0xff)
								{
									nx = x - ox;
									nz = z - oz;
								}
								if (x == ox + gr.px)
								{
									px = gr.px;
									break;
								}
							}
							else if (nx != -0xff && nz != -0xff)
							{
								px = x - ox - 1;
								break;
							}
						}
						if (nx != -0xff && nz != -0xff && px != -0xff) break;
					}
					// found the negative corner, now check the positive corner
					if (nx != -0xff && nz != -0xff && px != -0xff)
					{
						if (nx < -2 || px > 2 || nz < -2 || pz > 2)
							std::cout << "err" << std::endl;
						// looking for pz
						for (int32_t z = oz + nz, x = ox + nx; z <= oz + gr.pz; ++z)
						{
							chunk::WCoordChunk ccosc = ChunkCoordOfSComp({ x,z });
							if ((ccosc.wpos.x == wcc.wpos.x && ccosc.wpos.z == wcc.wpos.z))
							{
								if (z == gr.pz + oz)
								{
									pz = gr.pz;
									success = true;
									break;
								}
							}
							else
							{
								pz = z - oz - 1;
								success = true;
								break;
							}
						}
					}
					if (success)
					{
						switch (s->SType())
						{
						case structure_t::TREE:
							r.push_back(new Tree(wcc, { nx, px, nz, pz }, { ox,oz }, structure_t::TREE, s->Height(), tsize_t::BIG));
							break;
						}
					}
				}
			}
			return r;
		}
		chunk::WCoordChunk StructuresHandler::ChunkCoordOfSComp(const WVec2 v) const
		{
			signed int x, z;

			auto ccoord = [&](signed int a)->signed int
			{
				signed int absa = abs(a);
				if (a > 0) return (abs((a + 8) % 16)) == 0 ? (a + 16) / 16 : (absa + 8) * (a / absa) / 16;
				else return a == 0 || (abs((a + 8) % 16)) == 0 ? a / 16 : (absa + 8) * (a / absa) / 16;
			};

			x = ccoord(v.x);
			z = ccoord(v.z);

			return { { x, z } };
		}
		CVec2 StructuresHandler::SCompCoordInChunk(const chunk::WCoordChunk& wcc, WVec2 v) const
		{
			unsigned char x = wcc.wpos.x == 0 ? static_cast<unsigned char>(v.x + 8) :
				static_cast<unsigned char>(v.x - (wcc.wpos.x * 16 + 8 * (-wcc.wpos.x / wcc.wpos.x)));
			unsigned char z = wcc.wpos.z == 0 ? static_cast<unsigned char>(v.z + 8) :
				static_cast<unsigned char>(v.z - (wcc.wpos.z * 16 + 8 * (-wcc.wpos.z / wcc.wpos.z)));
			return { x , z };
		}
	}
}