#ifndef D3OBJ_HEADER
#define D3OBJ_HEADER

#include <GL/glew.h>
#include <glm/glm.hpp>

class D3Obj
{
public:
	explicit D3Obj(void) = default;
	void Init(const glm::vec3 translation)
	{
		m_translation = translation;

		static constexpr glm::vec3 COLOR = glm::vec3(1.0f, 0.5f, 0.0f);
		glm::vec3 verts[]
		{
			// top
			m_translation + glm::vec3(0.0f, 2.0f, 0.0f), COLOR,
			m_translation + glm::vec3(-1.0f, 0.0f, -1.0f), COLOR - 0.1f,
			m_translation + glm::vec3(-1.0f, 0.0f, 1.0f), COLOR - 0.2f,
			m_translation + glm::vec3(1.0f, 0.0f, -1.0f), COLOR - 0.3f,
			m_translation + glm::vec3(1.0f, 0.0f, 1.0f), COLOR - 0.4f
		};

		uint16_t indices[]
		{
			0, 1, 3,
			0, 1, 2,
			0, 2, 4,
			0, 3, 4,

			2, 1, 3, 2, 3, 4
		};

		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts) + sizeof(indices), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(verts), sizeof(indices), indices);

		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindVertexArray(0);
	}

	const uint32_t& VAO(void)
	{
		return m_vaoID;
	}
	const uint32_t& BufferID(void)
	{
		return m_bufferID;
	}
	void* Offset(void)
	{
		return (void*)(sizeof(glm::vec3) * 10);
	}
private:
	uint32_t m_bufferID;
	uint32_t m_vaoID;
	glm::vec3 m_translation;
};

#endif