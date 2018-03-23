#include "block_outline_buffer.h"

namespace minecraft
{
	namespace chunk
	{
		void BlockOutlineVAO::Init(void* d)
		{
			glGenVertexArrays(1, &m_vaoID);
			glBindVertexArray(m_vaoID);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		void BlockOutlineVAO::Bind(void) const
		{
			glBindVertexArray(m_vaoID);
		}
		void BlockOutlineVAO::UnBind(void) const
		{
			glBindVertexArray(0);
		}
		void BlockOutlineVAO::Destroy(void) const
		{
			glDeleteVertexArrays(1, &m_vaoID);
		}
	}
}