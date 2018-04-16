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
				: m_bufferID(0), m_indexOffset(nullptr), m_vao(new BlockOutlineVAO)
			{
				// calculate the indices
				Indices();
			}
			void CreateBuffer(void);
			void LoadVertices(void);
			void LoadIndices(void);
			VAO* Vao(void);
			void* Offset(void);
			const uint32_t BufferID(void);
		private:
			void Vertices(Vertex* arr);
			void Indices(void);
		private:
			VAO* m_vao;
			uint32_t m_bufferID;
			void* m_indexOffset;
			uint16_t m_indices[24];
		};
	}
}

#endif