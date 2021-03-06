#include "crosshair.h"

namespace minecraft
{
	namespace gui
	{
		Crosshair::Crosshair(const glm::vec2 p, const float s, TextureAtlas& ta)
			: GUI::GUI(6, p, s, new GUIVAO, ta)
		{
		}
		void Crosshair::Init(const glm::mat4& projection)
		{
			glm::vec4 proj0 = projection * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
			m_quad.cs[0].p = glm::vec2(proj0.x, proj0.y) * m_scale;
			glm::vec4 proj1 = projection * glm::vec4(-1.0f, +1.0f, 0.0f, 1.0f);
			m_quad.cs[1].p = glm::vec2(proj1.x, proj1.y) * m_scale;
			glm::vec4 proj2 = projection * glm::vec4(+1.0f, +1.0f, 0.0f, 1.0f);
			m_quad.cs[2].p = glm::vec2(proj2.x, proj2.y) * m_scale;
			glm::vec4 proj3 = projection * glm::vec4(+1.0f, -1.0f, 0.0f, 1.0f);
			m_quad.cs[3].p = glm::vec2(proj3.x, proj3.y) * m_scale;

			m_quad.cs[0].t = glm::vec2(+1.0f, +0.0f);
			m_quad.cs[1].t = glm::vec2(+0.0f, +0.0f);
			m_quad.cs[2].t = glm::vec2(+0.0f, +1.0f);
			m_quad.cs[3].t = glm::vec2(+1.0f, +1.0f);

			Quad quads[1] = {m_quad};
			std::array<uint16_t, 6> indexbones = { 0, 1, 2, 0, 2, 3 };
			m_buffer.Init(quads, 1, indexbones);
			m_vao->Init(m_quad.cs);
			//m_textureAtlas->Init();
			RenderDataInit();
		}
		void* Crosshair::IndexOffset(void)
		{
			return (void*)sizeof(Quad);
		}
		rnd::GLDrawElementsRenderDataAll Crosshair::DrawData(void)
		{
			return {m_rndData, 1};
		}
		void Crosshair::RenderDataInit(void)
		{
			rnd::GLDrawElementsRenderData d;
			d.bufferData = m_buffer.BufferID();
			d.offsetIndices = IndexOffset();
			d.vao = m_vao;
			d.t = m_textureAtlas->Tex();
			d.count = Count();
			m_rndData[0] = d;
		}
	}
}