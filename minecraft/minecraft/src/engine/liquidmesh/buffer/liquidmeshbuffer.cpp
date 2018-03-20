#include "liquidmeshbuffer.h"

namespace minecraft
{
	namespace lm
	{
		void LiquidMeshVAO::Init(void* d)
		{
			glGenVertexArrays(1, &m_vaoID);
			glBindVertexArray(m_vaoID);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glVertexAttribDivisor(0, 1);
		}
		void LiquidMeshVAO::Bind(void) const
		{
			glBindVertexArray(m_vaoID);
		}
		void LiquidMeshVAO::UnBind(void) const
		{
			glBindVertexArray(0);
		}
		void LiquidMeshVAO::Destroy(void) const
		{
			glDeleteVertexArrays(1, &m_vaoID);
		}

		// buffer class

		const uint32_t& LiquidMeshBuffer::BufferID(void)
		{
			return m_bufferID;
		}
		void LiquidMeshBuffer::Bind(void)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		}
		VAO* LiquidMeshBuffer::Vao(void)
		{
			return m_lmVAO;
		}
		void LiquidMeshBuffer::Load(std::size_t s, glm::vec3* positions)
		{
			glGenBuffers(1, &m_bufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * s, positions, GL_DYNAMIC_DRAW);
			
			m_lmVAO = new LiquidMeshVAO();
			m_lmVAO->Init(positions);
			m_lmVAO->UnBind();

			m_loaded = true;
		}
		bool LiquidMeshBuffer::Loaded(void)
		{
			return m_loaded;
		}
	}
}