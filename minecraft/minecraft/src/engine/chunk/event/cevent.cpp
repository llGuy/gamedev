#include "cevent.h"

namespace minecraft
{
	namespace chunk
	{
		ChunkEventHandler::ChunkEventHandler(void)
		{
		}
		void ChunkEventHandler::Event(const event_t ev, cmap::CMap& map, terrain::Terrain& t, 
			const glm::vec3& p, const glm::vec3& d)
		{
			switch (ev)
			{
			case event_t::DIG: Dig(p, d, map, t); return;
			case event_t::PLACE: Place(p, d, map); return;
			}
		}
		void ChunkEventHandler::Dig(const glm::vec3& p, const glm::vec3& d, cmap::CMap& map, terrain::Terrain& t)
		{
			glm::vec3 worldP = p + glm::vec3(0.0f, 0.25f, 0.0f);
			glm::vec3 worldD = d + glm::vec3(0.0f, 0.25f, 0.0f);
			for (ent::Ray r(worldD, worldP); r.Distance() < r.MaxDistance(); r.Extend(0.05f))
			{
				glm::vec3 rayPos = glm::round(r.EndPosition());
				Chunk::WCoordChunk wcc = ChunkCoordOfPoint(rayPos);
				CVec2 coordInChunk = PointCoordInChunk(wcc, rayPos);
				Chunk& c = map[wcc];
				bool exists = c.BlockExists(wcc.wpos, coordInChunk, rayPos);
				if (exists)
				{
					auto data = c.DestroyBlock(coordInChunk, rayPos.y, t);
					if (data.quant > 0)
					{
						for (uint32_t i = 0; i < data.quant; ++i)
						{
							Chunk::WCoordChunk cwcc = { { data.offsets[i].wcc.wpos.x, data.offsets[i].wcc.wpos.z } };
							Chunk& n = map[cwcc];
							n.LoadBlockFromGen(data.offsets[i].cv, data.offsets[i].y, t);
						}
					}
					return;
				}
			}
		}
		void ChunkEventHandler::BlockFaceRayIntersection(FaceIntersectionData* fid, uint32_t& ptr,
			ent::Ray& r, glm::vec3 face, uint32_t xyorz, float val, glm::vec3 round)
		{
			auto lerp = [&](const glm::vec3& a, const glm::vec3& b, uint32_t xyorz, float val)->FaceIntersectionData
			{
				glm::vec3 direction = b - a;
				float val1 = a[xyorz];
				float val2 = direction[xyorz];
				//float frac = (val - val1) / val2;
				float frac = (val - val1) / (val2);
				if (fabs(frac) > 1.0f) return { glm::vec3(-10.0f) };
				return { (direction * frac) + a, frac };
			};
			FaceIntersectionData fd1 = lerp(r.Position(), r.EndPosition(), xyorz, round[xyorz]);
			fd1.face = face;
			glm::vec3 l = glm::round(fd1.intersectionPoint);
			if (glm::all(glm::lessThan(glm::abs(l - round), glm::vec3(0.0001f))))
			{
 				fid[ptr] = fd1;
				++ptr;
			}
		}
		void ChunkEventHandler::Place(const glm::vec3& p, const glm::vec3& d, cmap::CMap& map)
		{
			glm::vec3 worldP = p + glm::vec3(0.0f, 0.25f, 0.0f);
			glm::vec3 worldD = d + glm::vec3(0.0f, 0.25f, 0.0f);
			for (ent::Ray r(worldD, worldP); r.Distance() < r.MaxDistance(); r.Extend(0.05f))
			{
				glm::vec3 rayPos = r.EndPosition();
				glm::vec3 round = glm::round(rayPos);
				Chunk::WCoordChunk wcc = ChunkCoordOfPoint(round);
				CVec2 coordInChunk = PointCoordInChunk(wcc, round);
				Chunk& c = map[wcc];
				bool exists = c.BlockExists(wcc.wpos, coordInChunk, rayPos);
				if (exists)
				{
					auto lerp = [&](const glm::vec3& a, const glm::vec3& b, uint32_t xyorz, float val)->FaceIntersectionData
					{
						glm::vec3 direction = b - a;
						float val1 = a[xyorz];
						float val2 = direction[xyorz];
						float frac = (val - val1) / val2;
						if (fabs(frac) > 1.0f) return { glm::vec3(-10.0f) };
						return { (direction * frac) + a, frac };
					};

					FaceIntersectionData fracs[2] = { {glm::vec3(), 10.0f, glm::vec3()},{ glm::vec3(), 10.0f, glm::vec3() } };
					uint32_t ptr = 0;
					// top face
					/*float topy = round.y;
					FaceIntersectionData fd1 = lerp(r.Position(), rayPos, 1, topy);
					fd1.face = glm::vec3(0.0f, 1.0f, 0.0f);
					glm::vec3 l = glm::round(fd1.intersectionPoint);
					if (glm::all(glm::lessThan(glm::abs(l - round), glm::vec3(0.0001f))))
					{
						fracs[ptr] = fd1;
						++ptr;
						c.PlaceBlock(PointCoordInChunk(wcc, round + glm::vec3(0.0f, 1.0f, 0.0f)), round.y);
						return;
					}*/
					enum
					{
						X,
						
						Y,

						Z
					};

					BlockFaceRayIntersection(fracs, ptr, r, glm::vec3(0.0f, 0.0f, 0.0f), Y, round.y, round);
					BlockFaceRayIntersection(fracs, ptr, r, glm::vec3(0.0f, -1.0f, 0.0f), Y, round.y - 1.0f, round - glm::vec3(0.0f, 1.0f, 0.0f));
					// check if there are multiple collision
					if (ptr == 2) goto END;

					BlockFaceRayIntersection(fracs, ptr, r, glm::vec3(0.0f, 0.0f, 0.0f), X, round.x, round);
					if (ptr == 2) goto END;
					BlockFaceRayIntersection(fracs, ptr, r, glm::vec3(-1.0f, 0.0f, 0.0f), X, round.x - 1.0f, round - glm::vec3(1.0f, 0.0f, 0.0f));
					if (ptr == 2) goto END;

					BlockFaceRayIntersection(fracs, ptr, r, glm::vec3(0.0f, 0.0f, 0.0f), Z, round.z, round);
					if (ptr == 2) goto END;
					BlockFaceRayIntersection(fracs, ptr, r, glm::vec3(0.0f, 0.0f, -1.0f), Z, round.z - 1.0f, round - glm::vec3(0.0f, 0.0f, 1.0f));
					if (ptr == 2) goto END;

					if (ptr > 0)
					{
					END:if (fracs[0].frac < fracs[1].frac)
						{
							ptr = 0;
						}
						else
						{
							ptr = 1;
						}

						c.PlaceBlock(PointCoordInChunk(wcc, round + fracs[ptr].face), round.y + fracs[ptr].face.y);
						return;
					}
				}
			}
		}
		const CVec2 ChunkEventHandler::PointCoordInChunk(const Chunk::WCoordChunk& wcc, const glm::vec3& v) const
		{
			uint8_t x = static_cast<uint8_t>(wcc.wpos.x == 0 ? v.x + 8 :
				v.x - (wcc.wpos.x * 16 + 8 * (-wcc.wpos.x / wcc.wpos.x)));
			uint8_t z = static_cast<uint8_t>(wcc.wpos.z == 0 ? v.z + 8 :
				v.z - (wcc.wpos.z * 16 + 8 * (-wcc.wpos.z / wcc.wpos.z)));
			return { x , z };
		}
		const Chunk::WCoordChunk ChunkEventHandler::ChunkCoordOfPoint(const glm::vec3& v) const
		{
			WVec2 xz = { static_cast<int32_t>(v.x), static_cast<int32_t>(v.z) };

			int32_t x, z;

			auto ccoord = [&](int32_t a)->int32_t
			{
				int32_t absa = abs(a);
				if (a > 0) return (abs((a + 8) % 16)) == 0 ? (a + 16) / 16 : (absa + 8) * (a / absa) / 16;
				else return a == 0 || (abs((a + 8) % 16)) == 0 ? a / 16 : (absa + 8) * (a / absa) / 16;
			};

			x = ccoord(xz.x);
			z = ccoord(xz.z);

			return { { x, z } };
		}
	}
}