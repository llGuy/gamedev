#include "buffer.h"
#include <vector>
#include <array>

namespace minecraft
{
	namespace gui
	{
		GUIBuffer::GUIBuffer(void)
		{
		}
		void GUIBuffer::Init(const Quad* q, uint32_t size, std::array<uint16_t, 6> ind)
		{
			glGenBuffers(1, &m_bufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Quad) * size + sizeof(uint16_t) * 6 * size, 0, GL_STATIC_DRAW);
			

			std::vector<uint16_t> indices;
			for (uint16_t i = 0; i < size; ++i)
			{
				uint16_t diff = i * 4;
				std::array<uint16_t, 6> is = 
				{
					ind[0] + diff, ind[1] + diff, ind[2] + diff,
					ind[3] + diff, ind[4] + diff, ind[5] + diff
				};

				indices.insert(indices.end(), is.begin(), is.end());
			}

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Quad) * size, q);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(Quad) * size, sizeof(uint16_t) * 6 * size, &indices[0]);
		}
		void GUIBuffer::Update(const Quad* q, uint32_t size, std::array<uint16_t, 6> ind)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			std::vector<uint16_t> indices;
			for (uint16_t i = 0; i < size; ++i)
			{
				uint16_t diff = i * 4;
				std::array<uint16_t, 6> is =
				{
					ind[0] + diff, ind[1] + diff, ind[2] + diff,
					ind[3] + diff, ind[4] + diff, ind[5] + diff
				};

				indices.insert(indices.end(), is.begin(), is.end());
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Quad) * size, q);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(Quad), sizeof(uint16_t) * 6 * size, &indices[0]);
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
			return m_bufferID;
		}
	}
}