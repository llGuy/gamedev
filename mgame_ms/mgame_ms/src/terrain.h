#ifndef _TERRAIN_HEADER_
#define _TERRAIN_HEADER_

#include <vector>
#include "buffer.h"
#include "entity.h"
#include "mesh.h"
#include "vao.h"

namespace mulgame {

	enum action_t
	{
		TERRAFORM,

		END_TERRAFORMING
	};

	struct ForcePoint
	{
		glm::ivec2 position;
		float intensity;
	};

	class Terrain
	{
	public:
		Terrain(void) = default;

		//void Handle(action_t, Entity& entity);
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
	private:
		static constexpr int32_t MESH_DIM = 64;
		Mesh<MESH_DIM, MESH_DIM> m_mesh;
	
		std::vector<ForcePoint> m_forcePoints;
	};

}

#endif
