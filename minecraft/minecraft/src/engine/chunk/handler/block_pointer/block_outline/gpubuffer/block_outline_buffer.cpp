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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			// color
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

		void BlockOutlineGPUBuffer::CreateBuffer(void)
		{
			glGenBuffers(1, &m_bufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8 + sizeof(uint16_t) * 24, 0, GL_DYNAMIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, sizeof(uint16_t) * 24, m_indices);
			m_vao->Init(nullptr);
		}
		void BlockOutlineGPUBuffer::LoadBuffer(glm::vec3 blockPosition)
		{
			Vertex verts[8];
			Vertices(verts, blockPosition);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 8, verts);
		}
		VAO* BlockOutlineGPUBuffer::Vao(void)
		{
			return m_vao;
		}
		void BlockOutlineGPUBuffer::Vertices(Vertex* arr, glm::vec3 center)
		{
			arr[0].pos = center + glm::vec3(+0.55f, +0.55f, +0.55f);
			arr[1].pos = center + glm::vec3(-0.55f, +0.55f, +0.55f);
			arr[2].pos = center + glm::vec3(-0.55f, +0.55f, -0.55f);
			arr[3].pos = center + glm::vec3(+0.55f, +0.55f, -0.55f);
				  
			arr[4].pos = center + glm::vec3(+0.55f, -0.55f, +0.55f);
			arr[5].pos = center + glm::vec3(-0.55f, -0.55f, +0.55f);
			arr[6].pos = center + glm::vec3(-0.55f, -0.55f, -0.55f);
			arr[7].pos = center + glm::vec3(+0.55f, -0.55f, -0.55f);

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
			m_indices[11] = 2;
			m_indices[12] = 2;
			m_indices[13] = 1;
			m_indices[14] = 2;
			m_indices[15] = 3;

			m_indices[16] = 0;
			m_indices[17] = 1;
			m_indices[18] = 0;
			m_indices[19] = 2;
			m_indices[20] = 2;
			m_indices[21] = 1;
			m_indices[22] = 2;
			m_indices[23] = 3;
		}
	}
}