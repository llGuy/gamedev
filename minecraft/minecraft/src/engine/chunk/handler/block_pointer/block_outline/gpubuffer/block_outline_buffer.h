#ifndef BLOCK_OUTLINE_GPUBUFFER_HEADER
#define BLOCK_OUTLINE_GPUBUFFER_HEADER

#include "../../../../../buffer/vao.h"

namespace minecraft
{
	namespace chunk
	{
		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 color;
		};

		class BlockOutlineVAO
			: public VAO
		{
		public:
			void Init(void* d) override;
			void Bind(void) const override;
			void UnBind(void) const override;
			void Destroy(void) const override;
		};

		class BlockOutlineGPUBuffer
		{
		public:
			explicit BlockOutlineGPUBuffer(void)
				: m_bufferID(0), m_indexOffset(nullptr), m_loaded(false)
			{
				// calculate the indices
				Indices();
			}
			void CreateBuffer(void);
			void LoadBuffer(glm::vec3 blockPosition);
			VAO* Vao(void);
		private:
			void Vertices(Vertex* arr, glm::vec3 center);
			void Indices(void);
		private:
			VAO* m_vao;
			uint32_t m_bufferID;
			void* m_indexOffset;
			uint16_t m_indices[24];
			bool m_loaded;
		};
	}
}

#endif