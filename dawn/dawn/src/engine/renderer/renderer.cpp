#include "renderer.h"

namespace dawn {

	void Renderer::DrawElements(const RenderParametersElements& params, const GLenum& mode, const GLenum& type) const
	{
		params.vao->Bind();
		params.ibuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(mode, params.indexCount, type, params.offset);
		params.vao->UnBind();
	}
	/*void Renderer::UniformData(const SHProgram& program, UData& data, const UDataLoc& loc) const
	{
		program.UniformMat4(data.projection, loc.projectionLoc);
		program.UniformMat4(data.model, loc.modelLoc);
		program.UniformMat4(data.view, loc.viewLoc);
	}*/

}