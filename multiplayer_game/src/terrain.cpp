#include "ray.h"
#include "terrain.h"

namespace mulgame {

    void Terrain::Handle(action_t action, Entity& entity)
    {
	for (Ray ray(entity.Direction(), entity.Eye(), 0.25f); ray.DistanceCovered() < ray.DistancetoEnd(); ray.Extend())
	{
	    float terrainHeightatRay = EntityHeight(ray.CurrentPosition());
	    const glm::vec3& currentRayPos = ray.CurrentPosition();
	    if (Equf(terrainHeightatRay, currentRayPos.y) || terrainHeightatRay > currentRayPos.y)
	    {
		int32_t x = static_cast<int32_t>(glm::round(currentRayPos.x)) - MESH_DIM;
		int32_t z = static_cast<int32_t>(glm::round(currentRayPos.z)) - MESH_DIM;

		std::cout << x << ";" << z << std::endl;
	    }
	}
    }

}
