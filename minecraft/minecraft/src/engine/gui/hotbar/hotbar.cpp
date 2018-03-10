#include "hotbar.h"

namespace minecraft
{
	namespace gui
	{
		Hotbar::Hotbar(const position_t& p, TextureAtlas& ta)
			: m_position(p), GUI::GUI(6, glm::vec2(0.0f), 1.0f, new GUIVAO, ta)
		{
		}
		void Hotbar::Init(const glm::mat4& projection) 
		{
			float xrad = 0.5f;
			m_stride = (xrad * 2) / 10.8f;
			float yrad = 0.06f;
			float offset = m_position * 0.8f;

			glm::vec4 nn = projection * glm::vec4(-xrad, -yrad, 0.0f, 0.0f);
			m_hotbar.cs[0].p = glm::vec2(nn.x, nn.y + offset);
			glm::vec4 np = projection * glm::vec4(-xrad, +yrad, 0.0f, 0.0f);
			m_hotbar.cs[1].p = glm::vec2(np.x, np.y + offset);
			glm::vec4 pp = projection * glm::vec4(xrad, yrad, 0.0f, 0.0f);
			m_hotbar.cs[2].p = glm::vec2(pp.x, pp.y + offset);
			glm::vec4 pn = projection * glm::vec4(xrad, -yrad, 0.0f, 0.0f);
			m_hotbar.cs[3].p = glm::vec2(pn.x, pn.y + offset);

			m_translation = nn;

			m_hotbar.cs[0].t = glm::vec2(+0.0f, +1.2f / 14.0f);
			m_hotbar.cs[2].t = glm::vec2(+1.0f / 14.0f * 10.0f, +0.0f);
			m_hotbar.cs[1].t = glm::vec2(+0.0f, +0.0f);
			m_hotbar.cs[3].t = glm::vec2(+1.0f / 14.0f * 10.0f, +1.2f / 14.0f);

			Quad quads[1] = {m_hotbar};
			std::array<uint16_t, 6> indexbones = { 0, 1, 2, 0, 2, 3};
			m_buffer.Init(quads, 1, indexbones);
			m_vao->Init(m_hotbar.cs);
			RenderDataInit();
		}
		void* Hotbar::IndexOffset(void)
		{
			return (void*)(sizeof(Quad));
		}
		const glm::vec2& Hotbar::Position(void)
		{
			return m_translation;
		}
		const float Hotbar::Stride(void)
		{
			return m_stride;
		}
		void Hotbar::RenderDataInit(void)
		{
			rnd::GLDrawElementsRenderData d;
			d.bufferData = m_buffer.BufferID();
			d.offsetIndices = IndexOffset();
			d.vao = m_vao;
			d.t = m_textureAtlas->Tex();
			d.count = Count();
			m_rndData[0] = d;
		}
		rnd::GLDrawElementsRenderDataAll Hotbar::DrawData(void)
		{
			rnd::GLDrawElementsRenderDataAll dataAll = { m_rndData, 1 };

			return dataAll;
		}
	}
}