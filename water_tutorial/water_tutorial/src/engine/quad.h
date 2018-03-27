#ifndef QUAD_HEADER
#define QUAD_HEADER

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Quad
{
public:
	Quad(void) = default;
	void Init(void)
	{
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * 2 + sizeof(uint16_t) * 6, 0, GL_STATIC_DRAW);

		static constexpr float RADIUS = 5.0f;
		glm::vec3 verts[] { glm::vec3(RADIUS, 0.0f, RADIUS), glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-RADIUS, 0.0f, RADIUS), glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(RADIUS, 0.0f, -RADIUS), glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-RADIUS, 0.0f, -RADIUS), glm::vec3(0.0f, 0.0f, 1.0f) };

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * 4 * 2, verts);
		
		uint16_t indices[] {3, 2, 1, 2, 0, 1};
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * 2, sizeof(uint16_t) * 6, indices);

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
private:
	uint32_t m_vaoID;
	uint32_t m_bufferID;
};

#endif