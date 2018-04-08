#include "renderer.h"

namespace dawn {

	void Renderer::DrawElements(const RenderParametersElements& params, GLenum mode, GLenum type) const
	{
		params.vao->Bind();
		params.ibuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(mode, params.indexCount, type, params.offset);
		params.vao->UnBind();
	}

}