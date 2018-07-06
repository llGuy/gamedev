#include "ray.h"
#include "terrain.h"

namespace mulgame {
    
    void Terrain::Handle(action_t action, Entity& entity)
    {
	switch (action)
	{
	case action_t::START_TERRAFORM:
	    if (entity.Terraforming() == -1)
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
			m_forcePoints.push_back({ ForcePoint( meshSpace, 10.0f ), entity.ID() });
			break;
		    }
		}
	    }
	    break;
	case action_t::END_TERRAFORMING:
	    // only if the entity is currently terraforming
	    if (entity.Terraforming() != -1)
	    {
		m_forcePoints.erase(m_forcePoints.begin() + entity.Terraforming());
		entity.Terraforming() = -1;
	    }
	    break;
	}
    }

    void Terrain::Handle(const ForcePoint& fp, Entity& entity, bool terraforming)
    {
	if (terraforming && entity.Terraforming() == -1)
	{
	    entity.Terraforming() = m_forcePoints.size();
	    m_forcePoints.push_back({ fp, entity.ID() });
	}
	
	if (!terraforming)
	{
	    if(entity.Terraforming() != -1)
	    {
		m_forcePoints.erase(m_forcePoints.begin() + entity.Terraforming());
		entity.Terraforming() = -1;
	    }
	}
    }

    void Terrain::UpdateFP(uint32_t fp, float timedelta)
    {
	ForcePoint& fpStruct = m_forcePoints[fp].fp;

	auto updateQuarter = [&](int32_t quotx, int32_t quotz, bool updateCenter, bool updateRadius) -> void
	    {
		for (uint32_t moundindex = 0; moundindex < m_mound.Size(); ++moundindex)
		{
		    auto mp = m_mound.At(moundindex, fpStruct.position, quotx, quotz);

		    bool updateVertex = true;
		    if (!updateRadius) 
			updateVertex &= !(mp.terrainSpacePos.x == fpStruct.position.x || mp.terrainSpacePos.y == fpStruct.position.y);
		    if (!updateCenter) 
			updateVertex &= !(mp.terrainSpacePos == fpStruct.position);
		    if (updateVertex)
		    {
			auto vertex = m_mesh.At(mp.terrainSpacePos.x, mp.terrainSpacePos.y);
			if (vertex.has_value())
			{
			    glm::vec3& v = *vertex.value();
			    float newHeight = v.y + mp.quotient * fpStruct.intensity;
			    if (v.y < newHeight)
				v.y += mp.quotient * fpStruct.intensity * timedelta;
			}
		    }
		}
	    };

	updateQuarter(1, 1, true, true);
	updateQuarter(1, -1, false, false);
	updateQuarter(-1, 1, false, false);
	updateQuarter(-1, -1, false, true);
    }

    void Terrain::UpdateForcePoints(float timedelta)
    {
	if (m_forcePoints.size() > 0)
	{
	    for (uint32_t i = 0; i < m_forcePoints.size(); ++i)
	    {
		UpdateFP(i, timedelta);
	    }
	    m_mesh.UpdateVertexBuffer();
	}
    }

}
