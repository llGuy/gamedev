#include "slotgui.h"

namespace minecraft
{
	namespace gui
	{
		SlotGUI::SlotGUI(const float& stride, const glm::vec2&,
			const glm::vec2&, TextureAtlas* t)
			: GUI::GUI(glm::vec2(0.0f), glm::vec2(0.0f), new GUIVAO, *t)
		{

		}
		void SlotGUI::CreateBlockImage(void)
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			glm::vec4 verts[8]
			{
				glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),	// negative x and z
				glm::vec4(-0.5f, +0.5f, -0.5f, 1.0f),
				
				glm::vec4(+0.5f, -0.5f, -0.5f, 1.0f),	// positive x and negative z
				glm::vec4(+0.5f, +0.5f, -0.5f, 1.0f),
				
				glm::vec4(-0.5f, -0.5f, +0.5f, 1.0f),	
				glm::vec4(-0.5f, +0.5f, +0.5f, 1.0f),
				
				glm::vec4(+0.5f, -0.5f, +0.5f, 1.0f),
				glm::vec4(+0.5f, +0.5f, +0.5f, 1.0f),
			};

			for (auto& i : verts)
			{
				i = rotationMatrix * i;
			}
			
			for (uint32_t i = 4; i < 8; ++i)
			{
				m_faces[0].cs[i].p = glm::vec2(verts[i].x, verts[i].y);
				
			}
		}
	}
}