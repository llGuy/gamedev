#include "ray.h"
#include "terrain.h"

namespace mulgame {

    void Terrain::Handle(action_t action, Entity& entity)
    {
	for (Ray ray(entity.Direction(), entity.Eye(), 0.25f, 6.0f); ray.DistanceCovered() < ray.DistancetoEnd(); ray.Extend())
	{
	    float terrainHeightatRay = EntityHeight(ray.CurrentPosition());
	    const glm::vec3& currentRayPos = ray.CurrentPosition();
	    if (Equf(terrainHeightatRay, currentRayPos.y) || terrainHeightatRay > currentRayPos.y)
	    {
		float x = glm::round(currentRayPos.x);
		float z = glm::round(currentRayPos.z);
		glm::ivec2 meshSpace = m_mesh.WorldtoMeshSpace(glm::vec2(x, z));
		std::cout << meshSpace.x << ";" << meshSpace.y << std::endl;
	    }
	}
    }

}
