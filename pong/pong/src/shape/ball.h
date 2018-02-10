#ifndef BALL_HEADER
#define BALL_HEADER

#include "shape.h"
#include <glm\gtx\transform.hpp>

class Ball
	: public Shape
{
public:
	explicit Ball(Color color, glm::vec3 translateVector, glm::vec3 ballDirection,
		const float top, const float bottom)
		: m_transformMatrix(1.0f),
		  m_translateVector(translateVector),
		  m_ballDirection(ballDirection),
		  m_ballSpeed(glm::vec3(0.01f, 0.01f, 0.0f)),
		  m_topGamePlane(top), m_bottomGamePlane(bottom)
	{
		glm::vec3 stackVerts[] = {
			glm::vec3(-m_radius, +m_radius, +m_radius), // 0
			color.m_colorPs, // Color
			glm::vec3(+m_radius, +m_radius, +m_radius), // 1
			color.m_colorPs, // Color
			glm::vec3(+m_radius, +m_radius, -m_radius), // 2
			color.m_colorPs, // Color
			glm::vec3(-m_radius, +m_radius, -m_radius), // 3
			color.m_colorPs, // Color
			
			glm::vec3(-m_radius, +m_radius, -m_radius), // 4
			color.m_colorPs, // Color
			glm::vec3(+m_radius, +m_radius, -m_radius), // 5
			color.m_colorPs, // Color
			glm::vec3(+m_radius, -m_radius, -m_radius), // 6
			color.m_colorPs, // Color
			glm::vec3(-m_radius, -m_radius, -m_radius), // 7
			color.m_colorPs, // Color
			
			glm::vec3(+m_radius, +m_radius, -m_radius), // 8
			color.m_colorFB, // Color
			glm::vec3(+m_radius, +m_radius, +m_radius), // 9
			color.m_colorFB, // Color
			glm::vec3(+m_radius, -m_radius, +m_radius), // 10
			color.m_colorFB, // Color
			glm::vec3(+m_radius, -m_radius, -m_radius), // 11
			color.m_colorFB, // Color
			
			glm::vec3(-m_radius, +m_radius, +m_radius), // 12
			color.m_colorFB, // Color
			glm::vec3(-m_radius, +m_radius, -m_radius), // 13
			color.m_colorFB, // Color
			glm::vec3(-m_radius, -m_radius, -m_radius), // 14
			color.m_colorFB, // Color
			glm::vec3(-m_radius, -m_radius, +m_radius), // 15
			color.m_colorFB, // Color
			
			glm::vec3(+m_radius, +m_radius, +m_radius), // 16
			color.m_colorSs, // Color
			glm::vec3(-m_radius, +m_radius, +m_radius), // 17
			color.m_colorSs, // Color
			glm::vec3(-m_radius, -m_radius, +m_radius), // 18
			color.m_colorSs, // Color
			glm::vec3(+m_radius, -m_radius, +m_radius), // 19
			color.m_colorSs, // Color
			
			glm::vec3(+m_radius, -m_radius, -m_radius), // 20
			color.m_colorSs, // Color
			glm::vec3(-m_radius, -m_radius, -m_radius), // 21
			color.m_colorSs, // Color
			glm::vec3(-m_radius, -m_radius, +m_radius), // 22
			color.m_colorSs, // Color
			glm::vec3(+m_radius, -m_radius, +m_radius), // 23
			color.m_colorSs, // Color
		};

		m_originalShapeVertices = { +m_radius, -m_radius, 
							+m_radius, -m_radius };

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

		unsigned int currentVertexBufferSize = Ball::VertexBufferSize();
		unsigned int currentIndexBufferSize = Ball::IndexBufferSize();

		memcpy(m_vertices, stackVerts, currentVertexBufferSize);
		memcpy(m_indices, stackIndices, currentIndexBufferSize);

		CreateBuffer(currentVertexBufferSize);
		CreateVertexArrayObject();
		CreateIndexBuffer(currentIndexBufferSize);

		UpdateShapeVertices();
	}
public:
	void Draw(glm::mat4 worldToViewMatrix, unsigned int uniformLocation,
		ShapeVertices* left = nullptr, ShapeVertices* right = nullptr) override
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBindVertexArray(m_vertexArrayID);

		Move(left, right);

		m_transformMatrix = worldToViewMatrix * glm::translate(m_translateVector);

		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &m_transformMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT, 0);
	}
protected:
	unsigned int VertexBufferSize(void) override
	{
		return m_numVertices * sizeof(glm::vec3);
	}
	unsigned int IndexBufferSize(void) override
	{
		return m_numIndices * sizeof(unsigned short);
	}
	void Move(ShapeVertices* left = nullptr, ShapeVertices* right = nullptr)
	{
		UpdateShapeVertices();
		if (YObstacleIsHit())
			ReverseYDirection();

		if (XObstacleIsHit(true, left))
			ReverseXDirection();
		if (XObstacleIsHit(false, right))
			ReverseXDirection();

		m_translateVector += m_ballDirection * m_ballSpeed; 
	}
	const bool YObstacleIsHit(void)
	{
		return (m_translateVector.y > m_topGamePlane || m_translateVector.y < m_bottomGamePlane);
	}
	const bool XObstacleIsHit(const bool isLeft, ShapeVertices* obstacle = nullptr)
	{
		if (obstacle == nullptr || obstacle == nullptr)
			Log("forgot to pass left paddle and right paddle vertices");

		if (isLeft)
		{
			if (fabs(obstacle->m_right - m_currentShapeVertices.m_left) < 0.0001f)
			{
				if (BallTopIsInBetweenPaddlePoles(obstacle) || BallBottomIsInBetweenPaddlePoles(obstacle))
				{
					return true;
				}
			}
		}
		else
		{
			if (fabs(obstacle->m_left - m_currentShapeVertices.m_right) < 0.0001f)
			{
				if (BallTopIsInBetweenPaddlePoles(obstacle) || BallBottomIsInBetweenPaddlePoles(obstacle))
				{
					return true;
				}
			}
		}
		return false;
	}
	const bool BallTopIsInBetweenPaddlePoles(ShapeVertices* obstacle)
	{
		return (m_currentShapeVertices.m_top < obstacle->m_top && m_currentShapeVertices.m_top > obstacle->m_bottom);
	}
	const bool BallBottomIsInBetweenPaddlePoles(ShapeVertices* obstacle)
	{
		return (m_currentShapeVertices.m_bottom < obstacle->m_top && m_currentShapeVertices.m_bottom > obstacle->m_bottom);
	}
	void ReverseYDirection(void)
	{
		m_ballDirection.y *= -1.0f;
	}
	void ReverseXDirection(void)
	{
		m_ballDirection.x *= -1.0f;
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
private:
	glm::vec3 m_translateVector;

	const float m_radius = 0.2f;

	glm::mat4 m_transformMatrix;

	glm::vec3 m_ballSpeed;
	glm::vec3 m_ballDirection;

	const float m_topGamePlane;
	const float m_bottomGamePlane;

	Shape::ShapeVertices m_originalShapeVertices;
	Shape::ShapeVertices m_currentShapeVertices;
};

#endif