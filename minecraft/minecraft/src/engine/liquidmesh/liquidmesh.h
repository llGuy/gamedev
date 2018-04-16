#ifndef LIQUID_MESH_HEADER
#define LIQUID_MESH_HEADER

#include <vector>
#include <glm/glm.hpp>

#include "buffer/liquidmeshbuffer.h"

namespace minecraft
{
	namespace lm
	{
		class LiquidMesh
		{
		public:
			explicit LiquidMesh(void) = default;

			const uint32_t Size(void);
			bool Loaded(void);
			bool Empty(void);
			void AddTile(const glm::vec3& v);
			VAO* Vao(void);
			void Load(void);
		private:
			std::vector<glm::vec3> m_tiles;
			LiquidMeshBuffer m_lmbuffer;
		};
	}
}

#endif