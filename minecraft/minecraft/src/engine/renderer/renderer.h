#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "../data/uni_data.h"
#include "../buffer/vao.h"

namespace minecraft
{
	namespace rnd
	{
		class Renderer
		{
		public:
			explicit Renderer(void) = default;
		public:
			/* for the chunks */
			void UniformData(data::CUData& cud, data::CUDataLocs& cudl) const;
		public:
			void ARender(GLenum mode, const VAO* vao,
				unsigned first, unsigned int count);
			void AInstancedRender(GLenum mode, const VAO* vao,
				unsigned first, unsigned int count, unsigned int primCount);
		};
	}
}

#endif