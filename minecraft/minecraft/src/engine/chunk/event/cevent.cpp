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
			case event_t::DIG: Dig(p, d, map, t); break;
			//case event_t::PLACE: Place(); break;
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
					c.DestroyBlock(coordInChunk, rayPos.y, t);
					return;
				}
			}
		}
		void ChunkEventHandler::Place(const glm::vec3& p, const glm::vec3& d, cmap::CMap& map)
		{
			glm::vec3 worldP = p + glm::vec3(0.0f, 0.25f, 0.0f);
			glm::vec3 worldD = d + glm::vec3(0.0f, 0.25f, 0.0f);
			for (ent::Ray r(worldD, worldP); r.Distance() < r.MaxDistance(); r.Extend(0.1f))
			{
				glm::vec3 rayPos = r.EndPosition();
				glm::vec3 round = glm::round(rayPos);
				Chunk::WCoordChunk wcc = ChunkCoordOfPoint(round);
				CVec2 coordInChunk = PointCoordInChunk(wcc, round);
				Chunk& c = map[wcc];
				bool exists = c.BlockExists(wcc.wpos, coordInChunk, rayPos);
				if (exists)
				{

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