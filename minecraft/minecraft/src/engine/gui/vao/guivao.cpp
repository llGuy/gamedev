#include "guivao.h"

namespace minecraft
{
	namespace gui
	{
		void GUIVAO::Init(void* d)
		{
			uint32_t size = sizeof(*static_cast<TPosition*>(d));
			glGenVertexArrays(1, &m_vaoID);
			glBindVertexArray(m_vaoID);

			// position
			glEnableVertexAttribArray(0);
			// texture coordinate
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, size, 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(sizeof(float) * 2));
		}
		void GUIVAO::Bind(void) const
		{
			glBindVertexArray(m_vaoID);
		}
		void GUIVAO::UnBind(void) const
		{
			glBindVertexArray(0);
		}
	}
}