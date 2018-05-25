#ifndef _TERRAIN_HEADER_
#define _TERRAIN_HEADER_

#include "buffer.h"
#include "mesh.h"
#include "vao.h"

namespace mulgame {

	class Terrain
	{
	public:
		Terrain(void) = default;

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
		Mesh<64, 64> m_mesh;
	};

}

#endif
