#ifndef LIQUID_MESH_BUFFER_HEADER
#define LIQUID_MESH_BUFFER_HEADER

#include <vector>
#include "../../buffer/vao.h"

namespace minecraft
{
	namespace lm
	{
		class LiquidMeshVAO
			: public VAO
		{
		public:
			void Init(void* d) override;
			void Bind(void) const override;
			void UnBind(void) const override;
			void Destroy(void) const override;
		};

		class LiquidMeshBuffer
		{
		public:
			explicit LiquidMeshBuffer(void)
				: m_bufferID(0), m_lmVAO(nullptr), m_loaded(false)
			{
			}

			bool Loaded(void);
			void Bind(void);
			VAO* Vao(void);
			const uint32_t& BufferID(void);
			void Load(std::size_t s, glm::vec3* positions);
		private:
			bool m_loaded;
			uint32_t m_bufferID;
			VAO* m_lmVAO;
		};
	}
}

#endif