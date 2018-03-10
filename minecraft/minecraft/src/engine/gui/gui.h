#ifndef GUI_HEADER
#define GUI_HEADER

#include <glm/glm.hpp>
#include "../../texture/texture.h"
#include "quad/buffer/buffer.h"
#include "vao/guivao.h"
#include "../block/block.h"
#include "../texture_atlas/texture_atlas.h"

namespace minecraft
{
	namespace gui
	{
		class GUI
		{
		public:
			virtual ~GUI(void)
			{
			}
			//explicit GUI(const glm::vec2, const glm::vec2 scale, VAO* vao, const std::string& f);
			//explicit GUI(const glm::vec2, const glm::vec2 scale, VAO* vao, const Texture& t);
			explicit GUI(const glm::vec2, const glm::vec2 scale, VAO* vao, TextureAtlas& ta);
			virtual void Init(const glm::mat4& projection);
			VAO* Vao(void);
			void BindBuffer(void);
			void BindTexture(void);
			uint32_t BufferID(void);
			TextureAtlas* Tex(void);

			virtual void* IndexOffset(void);
			virtual const glm::vec2& Position(void);
			// for the hotbar
			virtual const float Stride(void);
			virtual void Update(const int32_t& slot);
			virtual const Block::block_t& SelectedBlock(void);
		protected:
			glm::vec2 m_translation;
			glm::vec2 m_scale;

			VAO* m_vao;

			TextureAtlas* m_textureAtlas;

			GUIBuffer m_buffer;
		};
	}
}

#endif