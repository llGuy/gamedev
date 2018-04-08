#include "chunk.h"

namespace dawn { namespace terrain { namespace chunk {

			Chunk::Chunk(const ChunkSpaceCoord& coord)
				: m_mesh(coord)
			{
				m_mesh.GenerateData();
				m_mesh.CreateOpenGLObjs();
				m_mesh.RenderParams();
			}

			void Chunk::Init(void)
			{
				m_mesh.GenerateData();
				m_mesh.CreateOpenGLObjs();
				m_mesh.RenderParams();
			}

		}
	}
}