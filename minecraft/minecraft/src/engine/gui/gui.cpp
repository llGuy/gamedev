#include "gui.h"

namespace minecraft
{
	namespace gui
	{
		GUI::GUI(const glm::vec2 translation, const glm::vec2 scale, VAO* v, const std::string& f)
			: m_translation(translation), m_scale(scale), m_vao(v), m_texture(f)
		{
		}
		void GUI::Init(const glm::mat4& projection)
		{
		}
		VAO* GUI::Vao(void)
		{
			return m_vao;
		}
		void GUI::BindBuffer(void)
		{
			m_buffer.Bind();
		}
		void GUI::BindTexture(void)
		{
			m_texture.Bind(0);
		}
		uint32_t GUI::BufferID(void)
		{
			return m_buffer.BufferID();
		}
		Texture* GUI::Tex(void)
		{
			return &m_texture;
		}
	}
}