#include "renderer.h"
#include <iostream>

namespace minecraft
{
	namespace rnd
	{
		void Renderer::UniformData(data::CUData& cud, data::CUDataLocs& cudl) const
		{
			glUniformMatrix4fv(cudl.projectionMatrixLocation, 1, GL_FALSE, &cud.projectionMatrix[0][0]);
			glUniformMatrix4fv(cudl.viewMatrixLocation, 1, GL_FALSE, &cud.viewMatrix[0][0]);
			glUniform3fv(cudl.lightPositionLocation, 1, &cud.lightPosition[0]);
			glUniform3fv(cudl.eyePositionLocation, 1, &cud.eyePosition[0]);
			glUniform3fv(cudl.skyColorLocation, 1, &cud.skyColor[0]);
		}

		void Renderer::ARender(GLenum mode, const VAO* vao,
			unsigned first, unsigned int count)
		{
			vao->Bind();
			/* glDrawArrays() */
			vao->UnBind();
		}

		void Renderer::AInstancedRender(GLenum mode, const VAO* vao,
			unsigned first, unsigned int count, unsigned int primCount)
		{
			vao->Bind();
			glDrawArraysInstanced(mode, first, count, primCount);
			vao->UnBind();
		}
	}
}