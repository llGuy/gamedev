#include "liquidmesh.h"

namespace minecraft
{
	namespace lm
	{
		void LiquidMesh::AddTile(const glm::vec3& v)
		{
			m_tiles.push_back(v);
		}
		VAO* LiquidMesh::Vao(void)
		{
			return m_lmbuffer.Vao();
		}
		void LiquidMesh::Load(void)
		{
			m_lmbuffer.Load(m_tiles.size(), &m_tiles[0]);
		}
		bool LiquidMesh::Empty(void)
		{
			return m_tiles.size() == 0;
		}
		const uint32_t LiquidMesh::Size(void)
		{
			return m_tiles.size();
		}
		bool LiquidMesh::Loaded(void)
		{
			return m_lmbuffer.Loaded();
		}
	}
}