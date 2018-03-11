#ifndef CGPU_BUFFER_HEADER
#define CGPU_BUFFER_HEADER

#include <iostream>

#include "../gpublockdata.h"
#include "../../../buffer/vao.h"

namespace minecraft
{
	namespace chunk
	{
		namespace gpu
		{
			class CGPUVAO
				: public VAO
			{
			public:
				explicit CGPUVAO(void) = default;
				void Init(void* bd) override;
				void Bind(void) const override;
				void UnBind(void) const override;
				void Destroy(void) const override;
			};

			class CGPUBuffer
			{
			public:
				explicit CGPUBuffer(void)
					: m_createdVAO(false), m_vao(nullptr), m_bufferID(0)
				{
				}
				~CGPUBuffer(void)
				{
				}
				const uint32_t& BufferID(void);
				void Load(std::size_t components, BData* data);
				VAO* Vao(void);
				const bool CreatedVAO(void) const;
				void DeleteBuffer(void);
			private:
				void LoadBuffer(std::size_t components, BData* data);
				void UnBind(void) const;
			private:
				uint32_t m_bufferID;
				bool m_createdVAO;
				VAO* m_vao;
			};
		}
	}
}

#endif