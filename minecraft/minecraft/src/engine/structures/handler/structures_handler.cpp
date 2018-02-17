#include "structures_handler.h"

namespace minecraft
{
	namespace structures
	{
		StructuresHandler::StructuresHandler(signed int seed)
			: m_seed(seed)
		{
		}
		std::vector<Structure*> StructuresHandler::StructuresOfChunk(chunk::WCoordChunk wcc, WVec2 negCorner) const
		{
			std::vector<Structure*> v;

			signed int hcomp1 = wcc.wpos.x * 0x145;
			signed int hcomp2 = wcc.wpos.z * 0xf1a;
			signed int hash = std::hash<signed int>()(hcomp1 + hcomp2 + m_seed);
			srand(hash);
			unsigned int size = rand() % 3;
			v.resize(size);
			for (unsigned int i = 0; i < size; ++i)
			{
				unsigned char x = rand() % 16;
				unsigned char z = rand() % 16;

				// for now there are only trees
				v[i] = new Tree(wcc, { x, z }, { -2, 2, -2, 2 }, tsize_t::BIG, {negCorner.x + x, negCorner.z + z}, structure_t::TREE);
			}

			// neighbouring chunks

			// left
			for(signed int z = wcc.wpos.z - 1, x = wcc.wpos.x - 1; z <= wcc.wpos.z + 1; ++z)
			{
				std::vector<Structure*> n = StructuresOfNeighbouringChunks(wcc, { {x, z} }, {x * 16 - 8, z * 16 - 8});
				if (n.size() > 0) v.insert(v.end(), n.begin(), n.end());
			}
			// right
			for (signed int z = wcc.wpos.z - 1, x = wcc.wpos.x + 1; z <= wcc.wpos.z + 1; ++z)
			{
				std::vector<Structure*> n = StructuresOfNeighbouringChunks(wcc, { { x, z } }, { x * 16 - 8, z * 16 - 8 });
				if (n.size() > 0) v.insert(v.end(), n.begin(), n.end());
			}
			// top
			std::vector<Structure*> nt = StructuresOfNeighbouringChunks(wcc, { { wcc.wpos.x, wcc.wpos.z + 1 } }, 
				{ wcc.wpos.x * 16 - 8, (wcc.wpos.z + 1) * 16 - 8 });
			if (nt.size() > 0) v.insert(v.end(), nt.begin(), nt.end());

			// bottom
			std::vector<Structure*> nb = StructuresOfNeighbouringChunks(wcc, { { wcc.wpos.x, wcc.wpos.z - 1 } },
				{ wcc.wpos.x * 16 - 8, (wcc.wpos.z - 1) * 16 - 8 });
			if (nt.size() > 0) v.insert(v.end(), nt.begin(), nt.end());

			return v;
		}
		std::vector<Structure*> StructuresHandler::StructuresOfNeighbouringChunks(chunk::WCoordChunk wcc, 
			chunk::WCoordChunk nwcc, WVec2 negCorner) const
		{
			std::vector<Structure*> v = StructuresOfChunk(nwcc, negCorner);
			std::vector<Structure*> r; // result
			for (auto& s : v)
			{
				signed int ox = s->OriginW().x;
				signed int oz = s->OriginW().z;
				GenRange& gr = s->GRange();

				signed int nx, px, nz, pz;
				bool found = false;

				for (signed int z = oz + gr.nz; z <= oz + gr.pz; ++z)
				{
					for (signed int x = ox + gr.nx; x <= ox + gr.px; ++x)
					{
						chunk::WCoordChunk ccosc = ChunkCoordOfSComp({x,z});
						if (ccosc.wpos.x == wcc.wpos.x && ccosc.wpos.z == wcc.wpos.z)
						{
							nx = x - ox;
							nz = z - oz;
							found = true;
							if (x == ox + gr.px) 
							{ 
								px = gr.px;
								goto FINISH;
							}
							if (z == ox + gr.pz)
							{
								pz = gr.pz;
								goto FINISH;
							}
						}
						else
						{
							if (found)
							{
								px = x - ox;
								pz = z - oz;
								goto FINISH;
							}
						}
					}
				}
			FINISH:if (found)
				{
					switch (s->SType())
					{
					case structure_t::TREE:
						r.push_back(new Tree(wcc, { nx, px, nz, pz }, {ox,oz}, structure_t::TREE));
						break;
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