#ifndef CHUNK_EVENT_HEADER
#define CHUNK_EVENT_HEADER
#include <glm/glm.hpp>

#include "../../terrain/terrain.h"
#include "../../entities/player/raycast/ray.h"
#include "../map/cmap.h"
#include "../handler/block_pointer/block_pointer.h"

namespace minecraft
{
	namespace chunk
	{
		class ChunkEventHandler
		{
		public:
			enum class event_t
			{
				DIG,

				PLACE
			};

			explicit ChunkEventHandler(void);
			void Event(const event_t ev, cmap::CMap& map, terrain::Terrain& t, BlockPointer& bp, const glm::vec3& p = glm::vec3(0.0f),
				const glm::vec3& d = glm::vec3(0.0f), const Block::block_t& b = Block::block_t::INV);
			const glm::vec3 PointingAt(const glm::vec3& p, const glm::vec3& d, cmap::CMap& map);
		private:
			void Dig(const glm::vec3& p, const glm::vec3& d, cmap::CMap& map, terrain::Terrain& t, BlockPointer& bp);
			void Place(const glm::vec3& p, const glm::vec3& d, cmap::CMap& map, const Block::block_t& b, BlockPointer& bp);
			const CVec2 PointCoordInChunk(const Chunk::WCoordChunk& wcc, const glm::vec3& v) const;
			const Chunk::WCoordChunk ChunkCoordOfPoint(const glm::vec3& v) const;
			
			struct FaceIntersectionData
			{
				glm::vec3 intersectionPoint;
				float frac;
				glm::vec3 face;
			};
			void BlockFaceRayIntersection(FaceIntersectionData* fid, uint32_t& ptr,
				ent::Ray& r, glm::vec3 face, uint32_t xyorz, float val, glm::vec3 round);
		};
	}
}

#endif