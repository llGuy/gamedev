#ifndef PADDLE_HEADER
#define PADDLE_HEADER

#include "color\color.h"
#include "shape.h"

class Paddle
	: public Shape
{
public:
	explicit Paddle(Color color, glm::vec3 translateVector)
		: m_translateVector(translateVector)
	{
		glm::vec3 stackVerts[] = {
			glm::vec3(-m_middleToSide, +m_middleToPole, +m_middleToSide), // 0
			color.m_colorPs, // Color
			glm::vec3(+m_middleToSide, +m_middleToPole, +m_middleToSide), // 1
			color.m_colorPs, // Color
			glm::vec3(+m_middleToSide, +m_middleToPole, -m_middleToSide), // 2
			color.m_colorPs, // Color
			glm::vec3(-m_middleToSide, +m_middleToPole, -m_middleToSide), // 3
			color.m_colorPs, // Color

			glm::vec3(-m_middleToSide, +m_middleToPole, -m_middleToSide), // 4
			color.m_colorPs, // Color
			glm::vec3(+m_middleToSide, +m_middleToPole, -m_middleToSide), // 5
			color.m_colorPs, // Color
			glm::vec3(+m_middleToSide, -m_middleToPole, -m_middleToSide), // 6
			color.m_colorPs, // Color
			glm::vec3(-m_middleToSide, -m_middleToPole, -m_middleToSide), // 7
			color.m_colorPs, // Color

			glm::vec3(+m_middleToSide, +m_middleToPole, -m_middleToSide), // 8
			color.m_colorFB, // Color
			glm::vec3(+m_middleToSide, +m_middleToPole, +m_middleToSide), // 9
			color.m_colorFB, // Color
			glm::vec3(+m_middleToSide, -m_middleToPole, +m_middleToSide), // 10
			color.m_colorFB, // Color
			glm::vec3(+m_middleToSide, -m_middleToPole, -m_middleToSide), // 11
			color.m_colorFB, // Color

			glm::vec3(-m_middleToSide, +m_middleToPole, +m_middleToSide), // 12
			color.m_colorFB, // Color
			glm::vec3(-m_middleToSide, +m_middleToPole, -m_middleToSide), // 13
			color.m_colorFB, // Color
			glm::vec3(-m_middleToSide, -m_middleToPole, -m_middleToSide), // 14
			color.m_colorFB, // Color
			glm::vec3(-m_middleToSide, -m_middleToPole, +m_middleToSide), // 15
			color.m_colorFB, // Color

			glm::vec3(+m_middleToSide, +m_middleToPole, +m_middleToSide), // 16
			color.m_colorSs, // Color
			glm::vec3(-m_middleToSide, +m_middleToPole, +m_middleToSide), // 17
			color.m_colorSs, // Color
			glm::vec3(-m_middleToSide, -m_middleToPole, +m_middleToSide), // 18
			color.m_colorSs, // Color
			glm::vec3(+m_middleToSide, -m_middleToPole, +m_middleToSide), // 19
			color.m_colorSs, // Color

			glm::vec3(+m_middleToSide, -m_middleToPole, -m_middleToSide), // 20
			color.m_colorSs, // Color
			glm::vec3(-m_middleToSide, -m_middleToPole, -m_middleToSide), // 21
			color.m_colorSs, // Color
			glm::vec3(-m_middleToSide, -m_middleToPole, +m_middleToSide), // 22
			color.m_colorSs, // Color
			glm::vec3(+m_middleToSide, -m_middleToPole, +m_middleToSide), // 23
			color.m_colorSs, // Color
		};

		m_originalShapeVertices = { +m_middleToPole, -m_middleToPole,
							+m_middleToSide, -m_middleToSide };

		unsigned short stackIndices[] = {
			0,   1,  2,  0,  2,  3, // Top
			4,   5,  6,  4,  6,  7, // Front
			8,   9, 10,  8, 10, 11, // Right
			12, 13, 14, 12, 14, 15, // Left
			16, 17, 18, 16, 18, 19, // Back
			20, 22, 21, 20, 23, 22, // Bottom
		};

		m_numVertices = sizeof(stackVerts) / sizeof(glm::vec3);
		m_numIndices = sizeof(stackIndices) / sizeof(unsigned short);

		m_vertices = new glm::vec3[m_numVertices];
		m_indices = new unsigned short[m_numIndices];

		unsigned int currentVertexBufferSize = Paddle::VertexBufferSize();
		unsigned int currentIndexBufferSize = Paddle::IndexBufferSize();

		memcpy(m_vertices, stackVerts, currentVertexBufferSize);
		memcpy(m_indices, stackIndices, currentIndexBufferSize);

		CreateBuffer(currentVertexBufferSize);
		CreateVertexArrayObject();
		CreateIndexBuffer(currentIndexBufferSize);

		UpdateShapeVertices();
	}
public:
	Shape::ShapeVertices ShapeVerts(void) override
	{
		//std::cout << "PADDLE SHAPE VERTS : " << m_shapeVertices.m_right << " " << m_shapeVertices.m_top <<
			//m_shapeVertices.m_left << " " << m_shapeVertices.m_bottom << std::endl;

		return m_currentShapeVertices;
	}
	void Action(Shape::action_t action) override
	{
		if (action == MOVE_UP)
		{
			m_translateVector.y += m_paddleSpeed;
			UpdateShapeVertices();
		}
		if (action == MOVE_DOWN)
		{
			m_translateVector.y -= m_paddleSpeed;
			UpdateShapeVertices();
		}
	}
	void UpdateShapeVertices(void)
	{
		glm::vec4 vertsTop = glm::vec4(m_originalShapeVertices.m_right, m_originalShapeVertices.m_top, -1.0f, 1.0f);
		glm::vec4 vertsBottom = glm::vec4(m_originalShapeVertices.m_left, m_originalShapeVertices.m_bottom, -1.0f, 1.0f);

		glm::vec4 translatedTop = glm::translate(m_translateVector) * vertsTop;
		glm::vec4 translatedBottom = glm::translate(m_translateVector) * vertsBottom;

		m_currentShapeVertices = { translatedTop.y, translatedBottom.y,
			translatedTop.x, translatedBottom.x };
	}
	void Draw(glm::mat4 worldToViewMatrix, unsigned int uniformLocation,
		ShapeVertices* left = nullptr, ShapeVertices* right = nullptr) override
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBindVertexArray(m_vertexArrayID);

		m_transformMatrix = worldToViewMatrix * glm::translate(m_translateVector);
		
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &m_transformMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT, 0);
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
	const float m_middleToSide = 0.2f;
	const float m_middleToPole = 0.7f;

	const float m_paddleSpeed = 0.02f;

	glm::mat4 m_transformMatrix;
	glm::vec3 m_translateVector;

	Shape::ShapeVertices m_originalShapeVertices;
	Shape::ShapeVertices m_currentShapeVertices;
};

#endif