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

			// 3 * sizoef(float) => vec3
			// position
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
			// color
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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
		void BlockOutlineGPUBuffer::CreateBuffer(void)
		{
			glGenBuffers(1, &m_bufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8 + sizeof(uint16_t) * 24, 0, GL_STATIC_DRAW);
			LoadVertices();
			//glGenBuffers(1, &m_indexBufferID);
			LoadIndices();
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			m_vao->Init(nullptr);
		}
		void BlockOutlineGPUBuffer::LoadVertices(void)
		{
			Vertex verts[8];
			Vertices(verts);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 8, verts);
		}
		void BlockOutlineGPUBuffer::LoadIndices(void)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * 8, sizeof(uint16_t) * 24, m_indices);
		}
		VAO* BlockOutlineGPUBuffer::Vao(void)
		{
			return m_vao;
		}
		void* BlockOutlineGPUBuffer::Offset(void)
		{
			return (void*)(sizeof(Vertex) * 8);
		}
		void BlockOutlineGPUBuffer::Vertices(Vertex* arr)
		{
			static constexpr float RADIUS = 0.52f;
			arr[0].pos = glm::vec3(+RADIUS, +RADIUS, +RADIUS);
			arr[1].pos = glm::vec3(-RADIUS, +RADIUS, +RADIUS);
			arr[2].pos = glm::vec3(-RADIUS, +RADIUS, -RADIUS);
			arr[3].pos = glm::vec3(+RADIUS, +RADIUS, -RADIUS);
				  
			arr[4].pos = glm::vec3(+RADIUS, -RADIUS, +RADIUS);
			arr[5].pos = glm::vec3(-RADIUS, -RADIUS, +RADIUS);
			arr[6].pos = glm::vec3(-RADIUS, -RADIUS, -RADIUS);
			arr[7].pos = glm::vec3(+RADIUS, -RADIUS, -RADIUS);

			for (uint32_t i = 0; i < 8; ++i) arr[i].color = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		void BlockOutlineGPUBuffer::Indices(void)
		{
			// vertical lines
			m_indices[0] = 0; m_indices[1] = 4;
			m_indices[2] = 1; m_indices[3] = 5;
			m_indices[4] = 2; m_indices[5] = 6;
			m_indices[6] = 3; m_indices[7] = 7;

			// horizontal lines
			m_indices[8] = 0;
			m_indices[9] = 1;
			m_indices[10] = 0;
			m_indices[11] = 3;
			m_indices[12] = 2;
			m_indices[13] = 1;
			m_indices[14] = 2;
			m_indices[15] = 3;

			m_indices[16] = 4;
			m_indices[17] = 5;
			m_indices[18] = 4;
			m_indices[19] = 7;
			m_indices[20] = 6;
			m_indices[21] = 5;
			m_indices[22] = 6;
			m_indices[23] = 7;
		}
		const uint32_t BlockOutlineGPUBuffer::BufferID(void)
		{
			return m_bufferID;
		}
	}
}