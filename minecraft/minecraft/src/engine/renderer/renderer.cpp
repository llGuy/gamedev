#include "renderer.h"
#include <iostream>

namespace minecraft
{
	namespace rnd
	{
		void Renderer::UniformData(data::CUData& cud, data::CUDataLocs& cudl) const
		{
			glUniformMatrix4fv(cudl.modelMatrixLocation, 1, GL_FALSE, &cud.modelMatrix[0][0]);
			glUniformMatrix4fv(cudl.projectionMatrixLocation, 1, GL_FALSE, &cud.projectionMatrix[0][0]);
			glUniformMatrix4fv(cudl.viewMatrixLocation, 1, GL_FALSE, &cud.viewMatrix[0][0]);
			glUniform3fv(cudl.lightPositionLocation, 1, &cud.lightPosition[0]);
			glUniform3fv(cudl.eyePositionLocation, 1, &cud.eyePosition[0]);
			glUniform3fv(cudl.skyColorLocation, 1, &cud.skyColor[0]);

			if (cud.liquidMeshBlockType > -20.0f) glUniform1f(cudl.liquidBlockTypeLocation, cud.liquidMeshBlockType);
		}

		void Renderer::ARender(GLenum mode, const VAO* vao,
			uint32_t first, uint32_t count)
		{
			vao->Bind();
			glDrawArrays(mode, first, count);
			vao->UnBind();
		}

		void Renderer::AInstancedRender(GLenum mode, const VAO* vao,
			uint32_t first, uint32_t count, uint32_t primCount)
		{
			vao->Bind();
			glDrawArraysInstanced(mode, first, count, primCount);
			vao->UnBind();
		}

		/*		void Renderer::VecIMMRender(const debug::Line* l, const uint32_t size)
		{
			for (uint32_t i = 0; i < size; ++i)
			{
				glBegin(GL_LINES);
				glVertex3f(l[i].a.x, l[i].a.y, l[i].a.z);
				//glColor3f(l[i].color.r, l[i].color.g, l[i].color.b);
				glVertex3f(l[i].b.x, l[i].b.y, l[i].b.z);
				//glColor3f(l[i].color.r, l[i].color.g, l[i].color.b);
				glEnd();
			}
			}*/

		void Renderer::ERender(GLenum mode, const GLDrawElementsRenderData& d)
		{
			d.vao->Bind();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufferData);
			glDrawElements(mode, d.count, GL_UNSIGNED_SHORT, d.offsetIndices);
			d.vao->UnBind();
		}

		void Renderer::ERender(GLenum mode, uint32_t count, void* offset, VAO* vao, uint32_t buffer)
		{
			vao->Bind();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
			glDrawElements(mode, count, GL_UNSIGNED_SHORT, offset);
			vao->UnBind();
		}
	}
}
