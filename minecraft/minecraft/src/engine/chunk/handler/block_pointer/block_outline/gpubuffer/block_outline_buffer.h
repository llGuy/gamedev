#ifndef BLOCK_OUTLINE_GPUBUFFER_HEADER
#define BLOCK_OUTLINE_GPUBUFFER_HEADER

#include "../../../../../buffer/vao.h"

namespace minecraft
{
	namespace chunk
	{
		class BlockOutlineVAO
			: public VAO
		{
		public:
			void Init(void* d) override;
			void Bind(void) const override;
			void UnBind(void) const override;
			void Destroy(void) const override;
		};
	}
}

#endif