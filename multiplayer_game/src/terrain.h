#ifndef _TERRAIN_HEADER_
#define _TERRAIN_HEADER_

#include <vector>
#include "buffer.h"
#include "entity.h"
#include "mesh.h"
#include "vao.h"
#include "terrain_mound.h"

namespace mulgame {

    enum action_t
    {
	START_TERRAFORM,
	
	END_TERRAFORMING
    };

    struct ForcePoint
    {
	ForcePoint(void)
	    : position(-0xff, -0xff), intensity(-64.0f)
	{
	}

	ForcePoint(const glm::ivec2& pos, float i)
	    : position(pos), intensity(i)
	{
	}

	glm::ivec2 position;
	float intensity;
    };

    struct EntityMappedFP
    {
	ForcePoint fp;
	uint32_t entityID;
    };

    class Terrain
    {
    public:
	Terrain(void) = default;

	void Handle(action_t, Entity& entity);
	void Handle(const ForcePoint& fp, Entity&, bool);
	// plural
	void UpdateForcePoints(float timedelta);
    public:
	inline
	void Init(void)
	    {
		m_mesh.Init();
	    }
	inline
	DElementsData OGLBuffer(void)
	    {
		return m_mesh.OGLBuffer();
	    }
	inline
	VAO& VertexArray(void)
	    {
		return m_mesh.VertexArray();
	    }
	inline
	float EntityHeight(const glm::vec3& position)
	    {
		return m_mesh.HeightAtPoint(position.x, position.z);
	    }

	std::optional<EntityMappedFP*> At(int32_t entityID)
	{
	    for(auto& fp : m_forcePoints)
	    {
		if(fp.entityID == entityID) return &fp;
	    }
	    return std::optional<EntityMappedFP*> {};
	}

	// singular
	void UpdateFP(uint32_t fp, float timedelta);

	const ForcePoint& FP(int32_t index)
	{
	    return m_forcePoints[index].fp;
	}
    private:
	static constexpr int32_t MESH_DIM = 64;
	Mesh<MESH_DIM, MESH_DIM> m_mesh;

	// force point + id of entity that is terraforming
	std::vector<EntityMappedFP> m_forcePoints;

	Mound<5> m_mound;
    };

}

#endif
