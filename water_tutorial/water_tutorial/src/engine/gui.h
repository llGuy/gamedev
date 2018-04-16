#ifndef GUI_HEADER
#define GUI_HEADER

#include <GL/glew.h>
#include <glm/glm.hpp>

class GUI
{
public:
	explicit GUI(void) = default;
	void Init(glm::vec2 translation)
	{
		glm::vec2 verts[]
		{
			translation + glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f),
			translation + glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f),
			translation + glm::vec2(+0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
			translation + glm::vec2(+0.0f, +1.0f), glm::vec2(0.0f, 1.0f)
		};

		uint16_t indices[]
		{
			0, 1, 2, 1, 2, 3
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

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(glm::vec2)));

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
		return (void*)(sizeof(glm::vec2) * 8);
	}
private:
	uint32_t m_bufferID;
	uint32_t m_vaoID;
};

#endif