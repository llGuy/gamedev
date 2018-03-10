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
			};

			class CGPUBuffer
			{
			public:
				explicit CGPUBuffer(void)
					: m_createdVAO(false), m_vao(nullptr)
				{
				}
				~CGPUBuffer(void)
				{
					//if (m_vao != nullptr)
					//{
					//	std::cout << "deleted VAO" << std::endl;
					//	delete m_vao;
					//}
				}
				void Load(std::size_t components, BData* data);
				VAO* Vao(void);
				const bool CreatedVAO(void) const;
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