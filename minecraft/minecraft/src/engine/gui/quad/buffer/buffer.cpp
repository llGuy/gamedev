#include "buffer.h"

namespace minecraft
{
	namespace gui
	{
		GUIBuffer::GUIBuffer(void)
		{
		}
		void GUIBuffer::Init(const Quad& q)
		{
			glGenBuffers(1, &m_bufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			/*glBufferData(GL_ARRAY_BUFFER, sizeof(Quad) + sizeof(uint16_t) * 6, 0, GL_STATIC_DRAW);
			*/
			uint16_t indices[]
			{
				0, 1, 2,
				0, 2, 3
			};/*
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Quad), q.cs);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(Quad), sizeof(uint16_t) * 6, indices);*/
			uint32_t s = sizeof(Quad);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), q.cs, GL_STATIC_DRAW);

			glGenBuffers(1, &m_indexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}
		void GUIBuffer::Bind(void)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
		}
		void GUIBuffer::UnBind(void)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		uint32_t GUIBuffer::BufferID(void)
		{
			return m_indexBufferID;
		}
	}
}