#include "slotgui.h"

namespace minecraft
{
	namespace gui
	{
		//SlotGUI::SlotGUI(const float& stride, const glm::vec2&,
		//	const glm::vec2&, const Texture& t)
		//	: GUI::GUI(glm::vec2(0.0f), glm::vec2(0.0f), )
		//{

		//}
		void SlotGUI::CreateBlockImage(void)
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			glm::vec4 verts[8]
			{
				glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
				glm::vec4(-0.5f, +0.5f, -0.5f, 1.0f),
						
				glm::vec4(+0.5f, -0.5f, -0.5f, 1.0f),
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

		}
	}
}