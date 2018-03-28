#ifndef GUI_HEADER
#define GUI_HEADER

#include <GL/glew.h>
#include <glm/glm.hpp>

class GUI
{
public:
	explicit GUI(void) = default;
	void Init(void)
	{
		glm::vec2 verts[]
		{
			glm::vec2(-1.0f, 0.2f), glm::vec2(1.0f, 0.0f),
			glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f),
			glm::vec2(-0.2f, 1.0f), glm::vec2(0.0f, 1.0f),
			glm::vec2(-0.2f, 0.2f), glm::vec2(1.0f, 1.0f)
		};

		uint16_t indices[]
		{
			0, 1, 2, 2, 0, 3
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

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec2), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec2), (void*)(sizeof(glm::vec2)));

		glBindVertexArray(0);
	}
private:
	uint32_t m_bufferID;
	uint32_t m_vaoID;
};

#endif