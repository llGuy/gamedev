#ifndef LINE_HEADER
#define LINE_HEADER

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "shape.h"
#include "color\color.h"

class Line
	: public Shape
{
public:
	explicit Line(glm::vec3 vert1, glm::vec3 vert2, glm::vec3 translateVector, Color color)
	{
		glm::vec3 verts[]
		{
			vert1,
			color.m_colorFB,

			vert2,
			color.m_colorFB
		};

		m_numVertices = sizeof(verts) / sizeof(glm::vec3);

		m_vertices = new glm::vec3[m_numVertices];

		unsigned int currentVertexBufferSize = VertexBufferSize();

		memcpy(m_vertices, verts, currentVertexBufferSize);

		CreateBuffer(currentVertexBufferSize);
		CreateVertexArrayObject();

		m_translateMatrix = glm::translate(translateVector);
	}
public:
	void Draw(glm::mat4 worldToViewMatrix, unsigned int uniformLocation, Shape* apple = nullptr) override
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBindVertexArray(m_vertexArrayID);

		m_transformMatrix = worldToViewMatrix * m_translateMatrix;

		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &m_transformMatrix[0][0]);

		glDrawArrays(GL_LINES, 0, 2);
	}
private:
	unsigned int VertexBufferSize(void) override
	{
		return m_numVertices * sizeof(glm::vec3);
	}
	unsigned int IndexBufferSize(void) override
	{
		return m_numIndices * sizeof(unsigned short);
	}
private:
	glm::mat4 m_transformMatrix;
	glm::mat4 m_translateMatrix;
};

#endif