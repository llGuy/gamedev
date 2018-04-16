#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "../data/uni_data.h"
#include "../buffer/vao.h"
#include "../debug/debug.h"
#include "glderenderdata/glderenderdata.h"

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
				uint32_t first, uint32_t count);
			void AInstancedRender(GLenum mode, const VAO* vao,
				uint32_t first, uint32_t count, uint32_t primCount);
			void ERender(GLenum mode, const GLDrawElementsRenderData& d);
			void ERender(GLenum mode, uint32_t count, void* offset, VAO* vao, uint32_t buffer);

			/* immediate mode rendering */
			void VecIMMRender(const debug::Line* l, const uint32_t size);
		};
	}
}

#endif