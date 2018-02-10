#ifndef LINE_HEADER
#define LINE_HEADER

#include "shape.h"

class Line
	: public Shape
{
public:
	explicit Line(void)
		: m_vert1(1.0f, 0.0f, -7.0f), m_vert2(-1.0f, 0.0f, -7.0f)
	{
		glm::vec3 verts[]
		{
			m_vert1,
			glm::vec3(1.0f, 1.0f, 1.0f),

			m_vert2,
			glm::vec3(1.0f, 1.0f, 1.0f)
		};

		unsigned short indices[]
		{
			0, 1
		};

		m_vertices = new glm::vec3[sizeof(verts) / sizeof(glm::vec3)];
		m_indices = new unsigned short[2];

		memcpy(m_vertices, verts, sizeof(verts));
		memcpy(m_indices, indices, sizeof(indices));

		CreateBuffer(sizeof(verts));
		CreateIndexBuffer(sizeof(indices));
		CreateVertexArrayObject();
	}
public:
	void Draw(glm::mat4 worldToViewMatrix, unsigned int uniformLocation,
		ShapeVertices* left = nullptr, ShapeVertices* right = nullptr) override
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBindVertexArray(m_vertexArrayID);

		m_transformMatrix = worldToViewMatrix;

		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &m_transformMatrix[0][0]);

		glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_SHORT, 0);
	}
private:
	glm::vec3 m_vert1;
	glm::vec3 m_vert2;

	glm::mat4 m_transformMatrix;
};

#endif
