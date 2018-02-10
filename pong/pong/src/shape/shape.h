#ifndef SHAPE_HEADER
#define SHAPE_HEADER

#include "color\color.h"

#include <glm\glm.hpp>
#include <GL\glew.h>

class Shape
{
public:
	struct ShapeVertices
	{
		float m_top;
		float m_bottom;

		float m_right;
		float m_left;
	};
	virtual ~Shape(void)
	{
	}
	virtual void Draw(glm::mat4 worldToViewMatrix, unsigned int uniformLocation,
		ShapeVertices* left = nullptr, ShapeVertices* right = nullptr)
	{
	}
	//for paddles only
	enum action_t
	{
		MOVE_UP,

		MOVE_DOWN
	};
	virtual void Action(action_t action)
	{
	}
	virtual Shape::ShapeVertices ShapeVerts(void)
	{
		return { 0 };
	}
protected:
	virtual unsigned int VertexBufferSize(void)
	{
		return 0;
	}
	virtual unsigned int IndexBufferSize(void)
	{
		return 0;
	}
	void CreateBuffer(unsigned int vertexBufferSize)
	{
		glGenBuffers(1, &m_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, m_vertices, GL_STATIC_DRAW);
	}
	void CreateIndexBuffer(unsigned int indexBufferSize)
	{
		glGenBuffers(1, &m_indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, m_indices, GL_STATIC_DRAW);
	}
	void CreateVertexArrayObject(void)
	{
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBindVertexArray(m_vertexArrayID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	}
protected:
	unsigned int m_vertexBufferID;
	unsigned int m_indexBufferID;
	unsigned int m_vertexArrayID;

	glm::vec3* m_vertices;
	unsigned short* m_indices;

	unsigned int m_numVertices;
	unsigned int m_numIndices;
};

#endif