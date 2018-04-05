#ifndef RENDERER_HEADER
#define RENDERER_HEADER
#include <GL/glew.h>
#include "../buffer/buffer.h"
#include "../buffer/vao.h"
#include "render_params.h"

namespace dawn {

	class Renderer
	{
	public:
		Renderer(void) = default;
		Renderer(const Renderer&) = delete;
		const Renderer& operator=(const Renderer&) = delete;
	public:
		void DrawElements(const RenderParametersElements& params, const GLenum& mode, const GLenum& type = GL_UNSIGNED_SHORT) const;
	};

}

#endif