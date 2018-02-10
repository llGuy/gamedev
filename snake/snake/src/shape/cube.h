#ifndef CUBE_HEADER
#define CUBE_HEADER

#include <glm\gtx\transform.hpp>

#include "shape.h"
#include "color\color.h"

class Cube
	: public Shape
{
public:
	explicit Cube(Color color, float radius, glm::vec3 direction, glm::vec3 translateVector, float speed, 
		std::vector<Movement> pendingMovement, bool mobile = true)
		: m_translateVector(translateVector), m_radius(radius), m_cubeSpeed(speed), m_cubeDirection(direction),
		m_isChangingDirection(false), m_pendingMovements(pendingMovement), m_mobile(mobile)
	{	
		CreateVertices(color);
		CreateIndices();
		CreateBuffer(VertexBufferSize());
		CreateVertexArrayObject();
		CreateIndexBuffer(IndexBufferSize());
	}
public:
	void Draw(glm::mat4 viewProjectionMatrix, unsigned int location, Shape* apple = nullptr) override
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBindVertexArray(m_vertexArrayID);

		UpdateShapeVertices();

		if(m_mobile)
			PollDirectionChange();

		m_transformMatrix = viewProjectionMatrix * glm::translate(m_translateVector);

		glUniformMatrix4fv(location, 1, GL_FALSE, &m_transformMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT, 0);
	}
	Shape::ShapeVertices* ShapeVerts(void) override
	{
		return &m_currentShapeVertices;
	}
	std::vector<Shape::Movement> PendingMovements(void)
	{
		return m_pendingMovements;
	}
public:
	//movements
	void ToggleChangingDirection(glm::vec3 newDirection, glm::vec3 topRightChangingPoint) override
	{
		m_pendingMovements.push_back({newDirection, topRightChangingPoint,m_pendingMovements.size()});
	}
	void PollDirectionChange(void)
	{
		if (m_pendingMovements.size() > 0)
		{
			glm::vec3 currentTopRightPoint = glm::vec3(m_currentShapeVertices.m_right,
				m_currentShapeVertices.m_top, m_currentShapeVertices.m_front);
			if (fabs(currentTopRightPoint.x - m_pendingMovements[m_movementIndex].m_directitonChangingPoint.x) < 0.02f)
			{
				if (fabs(currentTopRightPoint.y - m_pendingMovements[m_movementIndex].m_directitonChangingPoint.y) < 0.02f)
				{
					if (fabs(currentTopRightPoint.z - m_pendingMovements[m_movementIndex].m_directitonChangingPoint.z) < 0.02f)
					{
						ChangeDirection();
						m_pendingMovements.erase(m_pendingMovements.begin() + m_movementIndex);
					}
				}
			}
		}
	}
	void UpdateShapeVertices(void) override
	{
		glm::vec4 vertsTop = glm::vec4(m_originalShapeVertices.m_right, m_originalShapeVertices.m_top, m_originalShapeVertices.m_front, 1.0f);
		glm::vec4 vertsBottom = glm::vec4(m_originalShapeVertices.m_left, m_originalShapeVertices.m_bottom, m_originalShapeVertices.m_back, 1.0f);

		glm::vec4 translatedTop = glm::translate(m_translateVector) * vertsTop;
		glm::vec4 translatedBottom = glm::translate(m_translateVector) * vertsBottom;

		m_currentShapeVertices = { translatedTop.y, translatedBottom.y,
			translatedTop.x, translatedBottom.x,
			translatedTop.z, translatedBottom.z };
	}
	void ChangeDirection(void) override
	{
		m_cubeDirection = m_pendingMovements[m_movementIndex].m_nextDirection;
	}
	glm::vec3* TranslateVector(void) override
	{
		return &m_translateVector;
	}
	glm::vec3 Direction(void) override
	{
		return m_cubeDirection;
	}
	const bool DetectedCollisionOnXExtremities(Shape::ShapeVertices shapeVertsOfOtherShape)
	{
		float extOfThis[4] = { m_currentShapeVertices.m_top, m_currentShapeVertices.m_bottom,
			m_currentShapeVertices.m_front, m_currentShapeVertices.m_back };
		float extOfOther[4] = { shapeVertsOfOtherShape.m_top, shapeVertsOfOtherShape.m_bottom,
			shapeVertsOfOtherShape.m_front, shapeVertsOfOtherShape.m_back };
		if (m_cubeDirection.x < -0.1f)
		{
			// compare with the other's right side
			float sideOfThis = m_currentShapeVertices.m_left;
			float sideOfOther = shapeVertsOfOtherShape.m_right;

			if (PointContainedInPlane(sideOfThis, sideOfOther, extOfThis, extOfOther))
				return true;
		}
		else if (m_cubeDirection.x > 0.1f)
		{
			// compare with the other's left side
			float sideOfThis = m_currentShapeVertices.m_right;
			float sideOfOther = shapeVertsOfOtherShape.m_left;

			if (PointContainedInPlane(sideOfThis, sideOfOther, extOfThis, extOfOther))
				return true;
		}
		return false;
	}
	const bool DetectedCollisionOnYExtremities(Shape::ShapeVertices shapeVertsOfOtherShape)
	{
		float extOfThis[4] = { m_currentShapeVertices.m_right, m_currentShapeVertices.m_left,
			m_currentShapeVertices.m_front, m_currentShapeVertices.m_back };
		float extOfOther[4] = { shapeVertsOfOtherShape.m_right, shapeVertsOfOtherShape.m_left,
			shapeVertsOfOtherShape.m_front, shapeVertsOfOtherShape.m_back };
		if (m_cubeDirection.y < -0.1f)
		{
			// compare with the other's top side
			float sideOfThis = m_currentShapeVertices.m_bottom;
			float sideOfOther = shapeVertsOfOtherShape.m_top;

			if (PointContainedInPlane(sideOfThis, sideOfOther, extOfThis, extOfOther))
				return true;
		}
		else if (m_cubeDirection.y > 0.1f)
		{
			// compare with the other's bottom side
			float sideOfThis = m_currentShapeVertices.m_top;
			float sideOfOther = shapeVertsOfOtherShape.m_bottom;

			if (PointContainedInPlane(sideOfThis, sideOfOther, extOfThis, extOfOther))
				return true;
		}
	}
	const bool DetectedCollisionOnZExtremities(Shape::ShapeVertices shapeVertsOfOtherShape)
	{
		float extOfThis[4] = { m_currentShapeVertices.m_right, m_currentShapeVertices.m_left,
			m_currentShapeVertices.m_top, m_currentShapeVertices.m_bottom };
		float extOfOther[4] = { shapeVertsOfOtherShape.m_right, shapeVertsOfOtherShape.m_left,
			shapeVertsOfOtherShape.m_top, shapeVertsOfOtherShape.m_bottom };
		if (m_cubeDirection.z < -0.1f)
		{
			// compare with the other's front side
			float sideOfThis = m_currentShapeVertices.m_back;
			float sideOfOther = shapeVertsOfOtherShape.m_front;

			if (PointContainedInPlane(sideOfThis, sideOfOther, extOfThis, extOfOther))
				return true;
		}
		else if (m_cubeDirection.z > 0.1f)
		{
			// compare with the other's back side
			float sideOfThis = m_currentShapeVertices.m_front;
			float sideOfOther = shapeVertsOfOtherShape.m_back;

			if (PointContainedInPlane(sideOfThis, sideOfOther, extOfThis, extOfOther))
				return true;
		}
	}
	const bool DetectCollision(Shape* other) override
	{
		Shape::ShapeVertices shapeVertsOfOtherShape = *(other->ShapeVerts());
		if (fabs(m_cubeDirection.x) > 0.1f)
		{
			if (DetectedCollisionOnXExtremities(shapeVertsOfOtherShape))
				return true;
		}
		else if (fabs(m_cubeDirection.y) > 0.1f)
		{
			if (DetectedCollisionOnYExtremities(shapeVertsOfOtherShape))
				return true;
		}
		else if (fabs(m_cubeDirection.z) > 0.1f)
		{
			if (DetectedCollisionOnZExtremities(shapeVertsOfOtherShape))
				return true;
		}
		return false;
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
	void CreateVertices(Color color)
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
			+m_radius, -m_radius,
			+m_radius, -m_radius };
		m_numVertices = sizeof(stackVerts) / sizeof(glm::vec3);
		m_vertices = new glm::vec3[m_numVertices];
		unsigned int currentVertexBufferSize = VertexBufferSize();
		memcpy(m_vertices, stackVerts, currentVertexBufferSize);
	}
	void CreateIndices(void)
	{
		unsigned short indices[]
		{
			0,   1,  2,  0,  2,  3, // Top
			4,   5,  6,  4,  6,  7, // Front
			8,   9, 10,  8, 10, 11, // Right
			12, 13, 14, 12, 14, 15, // Left
			16, 17, 18, 16, 18, 19, // Back
			20, 22, 21, 20, 23, 22, // Bottom
		};

		m_numIndices = sizeof(indices) / sizeof(unsigned short);
		m_indices = new unsigned short[m_numIndices];
		unsigned int currentIndexBufferSize = IndexBufferSize();
		memcpy(m_indices, indices, currentIndexBufferSize);
	}
	void Move(void) override
	{
		m_translateVector += m_cubeDirection * m_cubeSpeed;
	}
	const bool PointContainedInPlane(float faceThis, float faceOther, float vertsOfThis[4], float vertsOfOther[4])
	{
		if (fabs(faceThis - faceOther) < 0.01f)
		{
			if (IsInBetween(vertsOfThis[0], vertsOfOther[0], vertsOfOther[1]))
				if (IsInBetween(vertsOfThis[2], vertsOfOther[2], vertsOfOther[3]))
					return true;
			if (IsInBetween(vertsOfThis[1], vertsOfOther[0], vertsOfOther[1]))
				if (IsInBetween(vertsOfThis[3], vertsOfOther[2], vertsOfOther[3]))
					return true;
		}
		return false;
	}
	const bool IsInBetween(float point, float ext1, float ext2)
	{
		if (point < ext1 + 0.01f)
			if (point > ext2 - 0.01f)
				return true;
		return false;
	}
private:
	float m_radius;
	float m_cubeSpeed;
	glm::mat4 m_transformMatrix;
	glm::vec3 m_translateVector;
	glm::vec3 m_cubeDirection;
	Shape::ShapeVertices m_currentShapeVertices;
	Shape::ShapeVertices m_originalShapeVertices;
	bool m_isChangingDirection;
	glm::vec3 m_topRightChangingPoint;
	std::vector<Movement> m_pendingMovements;
	unsigned short m_movementIndex = 0;
	bool m_mobile;
};

#endif