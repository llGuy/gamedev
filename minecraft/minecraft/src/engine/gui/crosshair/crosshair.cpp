#include "crosshair.h"

namespace minecraft
{
	namespace gui
	{
		Crosshair::Crosshair(const glm::vec2 p, const glm::vec2 s)
			: GUI::GUI(p, s, new GUIVAO, "res\\textures\\gui\\crosshair.png")
		{
		}
		void Crosshair::Init(const glm::mat4& projection)
		{
			m_quad.cs[0].p = glm::vec2(-1.0f, -1.0f);
			m_quad.cs[1].p = glm::vec2(-1.0f, +1.0f);
			m_quad.cs[2].p = glm::vec2(+1.0f, +1.0f);
			m_quad.cs[3].p = glm::vec2(+1.0f, -1.0f);

			m_quad.cs[0].t = glm::vec2(+0.0f, +0.0f);
			m_quad.cs[1].t = glm::vec2(+0.0f, +1.0f);
			m_quad.cs[2].t = glm::vec2(+1.0f, +0.0f);
			m_quad.cs[3].t = glm::vec2(+1.0f, +1.0f);

			m_buffer.Init(m_quad);
			m_vao->Init(m_quad.cs);
			m_texture.Init();
		}
	}
}