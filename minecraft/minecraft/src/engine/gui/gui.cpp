#include "gui.h"

namespace minecraft
{
	namespace gui
	{
		//GUI::GUI(const glm::vec2 translation, const glm::vec2 scale, VAO* v, const std::string& f)
		//	: m_translation(translation), m_scale(scale), m_vao(v), m_texture(f)
		//{
		//}
		//GUI::GUI(const glm::vec2 translation, const glm::vec2 scale, VAO* vao, const Texture& t)
		//	: m_translation(translation), m_scale(scale), m_vao(vao), m_texture(t)
		//{
		//}
		GUI::GUI(uint32_t count, const glm::vec2 translation, const float scale, VAO* v, TextureAtlas& ta)
			: m_translation(translation), m_scale(scale), m_vao(v), m_textureAtlas(&ta), m_count(count)
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
			m_textureAtlas->Bind(0);
		}
		uint32_t GUI::BufferID(void)
		{
			return m_buffer.BufferID();
		}
		TextureAtlas* GUI::Tex(void)
		{
			return m_textureAtlas;
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
		const Block::block_t GUI::SelectedBlock(void)
		{
			return Block::block_t::INV;
		}
		uint32_t GUI::Count(void)
		{
			return m_count;
		}
		rnd::GLDrawElementsRenderDataAll GUI::DrawData(void)
		{
			return rnd::GLDrawElementsRenderDataAll();
		}
	}
}