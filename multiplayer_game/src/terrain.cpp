#include "ray.h"
#include "terrain.h"

namespace mulgame {

    void Terrain::Handle(action_t action, Entity& entity)
    {
	switch(action)
	{
	case action_t::START_TERRAFORM:
	    if(entity.Terraforming() != -1)
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

			entity.Terraforming() = m_forcePoints.size();
			m_forcePoints.push_back(ForcePoint{meshSpace, 0.5f} );
		    }
		}
	    }
	    break;
	case action_t::END_TERRAFORMING:
	    m_forcePoints.erase(m_forcePoints.begin() + entity.Terraforming());
	    entity.Terraforming() = -1;
	    break;
	}
    }

    void Terrain::UpdateFP(uint32_t fp, float timedelta)
    {
	ForcePoint& fpStruct = m_forcePoints[fp];
	glm::vec3& vertex = m_mesh.At(fpStruct.position.x, fpStruct.position.y);
	vertex.y += timedelta * fpStruct.intensity;
    }

    void Terrain::UpdateForcePoints(float timedelta)
    {
	if(m_forcePoints.size() > 0)
	{
	    for(uint32_t i = 0; i < m_forcePoints.size(); ++i)
	    {
		UpdateFP(i, timedelta);
	    }
	    m_mesh.UpdateVertexBuffer();
	}
    }

}
