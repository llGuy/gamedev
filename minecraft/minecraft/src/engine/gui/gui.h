#ifndef GUI_HEADER
#define GUI_HEADER

#include <glm/glm.hpp>
#include "../../texture/texture.h"
#include "quad/buffer/buffer.h"
#include "vao/guivao.h"

namespace minecraft
{
	namespace gui
	{
		class GUI
		{
		public:
			explicit GUI(const glm::vec2, const glm::vec2 scale, VAO* vao, const std::string& f);
			virtual void Init(const glm::mat4& projection);
			VAO* Vao(void);
			void BindBuffer(void);
			void BindTexture(void);
			uint32_t BufferID(void);
			Texture* Tex(void);
		protected:
			glm::vec2 m_translation;
			glm::vec2 m_scale;

			VAO* m_vao;
			Texture m_texture;
			GUIBuffer m_buffer;
		};
	}
}

#endif