#ifndef CHUNK_EVENT_HEADER
#define CHUNK_EVENT_HEADER
#include <glm/glm.hpp>

#include "../../terrain/terrain.h"
#include "../../entities/player/raycast/ray.h"
#include "../map/cmap.h"

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
			void Event(const event_t ev, cmap::CMap& map, terrain::Terrain& t, const glm::vec3& p = glm::vec3(0.0f),
				const glm::vec3& d = glm::vec3(0.0f));
		private:
			void Dig(const glm::vec3& p, const glm::vec3& d, cmap::CMap& map, terrain::Terrain& t);
			void Place(const glm::vec3& p, const glm::vec3& d, const cmap::CMap& map);
			const CVec2 PointCoordInChunk(const Chunk::WCoordChunk& wcc, const glm::vec3& v) const;
			const Chunk::WCoordChunk ChunkCoordOfPoint(const glm::vec3& v) const;
		};
	}
}

#endif