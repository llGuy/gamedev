#include "gui.h"

namespace minecraft
{
	namespace gui
	{
		GUI::GUI(const glm::vec2 translation, const glm::vec2 scale, VAO* v, const std::string& f)
			: m_translation(translation), m_scale(scale), m_vao(v), m_texture(f)
		{
		}
		GUI::GUI(const glm::vec2 translation, const glm::vec2 scale, VAO* vao, const Texture& t)
			: m_translation(translation), m_scale(scale), m_vao(vao), m_texture(t)
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
		void* GUI::IndexOffset(void)
		{
			return nullptr;
		}
		const glm::vec2& GUI::Position(void)
		{
			return m_translation;
		}
		const float GUI::Stride(void)
		{
			return 0.0f;
		}
		void GUI::Update(const int32_t& slot)
		{
			// donoth
		}
		const Block::block_t& GUI::SelectedBlock(void)
		{
			return Block::block_t::INV;
		}
	}
}