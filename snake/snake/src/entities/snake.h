#ifndef SNAKE_HEADER
#define SNAKE_HEADER

#include <vector>
#include <math.h>
#include <iostream>

#include "..\shape\cube.h"
#include "apple.h"

class Snake
{
public:
	struct LiveVector
	{
		float* m_x;
		float* m_y;
		float* m_z;
	};
	enum movement_t
	{	
		MOVE_LEFT,

		MOVE_RIGHT,

		MOVE_UP,

		MOVE_DOWN
	};
	explicit Snake(float speed, float radiusOfGrid, Apple* apple)
		: m_xzDirectionOfSnake(1.0f, 0.0f, 0.0f), 
		m_xyzDirectionOfSnake(1.0f, 0.0f, 0.0f), m_isChangingDirection(false),
		m_colorDelta(0.05f, 0.05f, 0.05f), m_indexRightVectors(0), m_indexLeftVectors(0),
		m_isMovingInAltitude(false), m_speed(speed), m_radiusOfGrid(radiusOfGrid, radiusOfGrid, radiusOfGrid),
		m_currentBaseColor(GREEN)
	{
		Init(apple);
	}
public:
	void Draw(glm::mat4 viewProjectionMatrix, unsigned int location, Apple* apple, bool* lost)
	{
		MoveTheCubes();
		DrawTheCubes(viewProjectionMatrix, location);
		if (IsSnakeDead())
		{
			*lost = true;
			std::cout << "\nsnake died!\n\n";
		}
		if (SurpassedLimit())
		{
			*lost = true;
			std::cout << "\nsnake went out of bounds\n\n";
		}

		SnakeAteApple(apple);
	}
	Shape* Head(void)
	{
		return m_cubes[m_headCube];
	}
	void Move(movement_t movement, Apple* apple)
	{
		if (movement == MOVE_RIGHT)
			MoveRight();
		else if (movement == MOVE_LEFT)
			MoveLeft();
		else if (movement == MOVE_UP)
			MoveUp();
		else if (movement == MOVE_DOWN)
			MoveDown();
		GetCompareVector(apple);
	}
private:
	void MoveTheCubes(void)
	{
		for (auto& cubeIter : m_cubes)
			cubeIter->Move();
	}
	void DrawTheCubes(glm::mat4 viewProjectionMatrix, unsigned int location)
	{
		for (auto& cubeIter : m_cubes)
			cubeIter->Draw(viewProjectionMatrix, location);
	}
	const bool IsSnakeDead(void)
	{
		for (unsigned int cubeIter = 1; cubeIter < m_cubes.size(); ++cubeIter)
			if (m_cubes[0]->DetectCollision(m_cubes[cubeIter]))
				return true;
		return false;
	}
	void AddCube(void)
	{
		if (IsAtLimitOfCurrentBaseColor())
			ChangeBaseColor();
		Color color(static_cast<color_t>(m_currentBaseColor));
	
		color.m_colorFB -= m_colorDelta;	//front back
		color.m_colorPs -= m_colorDelta;	//poles
		color.m_colorSs -= m_colorDelta;	//sides

		//copy the current last cube's data
		std::vector<Cube::Movement> pendingMovementsOfPreviousCube = m_cubes[m_cubes.size() - 1]->PendingMovements();
		m_cubes.push_back(new Cube(color, 0.5f, m_cubes[m_cubes.size() - 1]->Direction(), 
			*m_cubes[m_cubes.size() - 1]->TranslateVector() - m_cubes[m_cubes.size() - 1]->Direction(), m_speed, pendingMovementsOfPreviousCube));
		m_colorDelta += 0.1f;
	}
	void ChangeBaseColor(void)
	{
		// the m_currentBaseColor will cycle through colors
		if (m_currentBaseColor == MAGENTA) m_currentBaseColor = GREEN;
		else m_currentBaseColor++;
		m_colorDelta = glm::vec3(0.0f);
	}
	const bool IsAtLimitOfCurrentBaseColor(void)
	{
		return glm::all(glm::greaterThan(m_colorDelta, glm::vec3(0.7f)));
	}
	//left and right
	void ChangeToNewDirectionLR(unsigned short& newDirectionIndex, unsigned short& otherDirectionIndex, glm::vec3 turningVectors[4])
	{
		m_xzDirectionOfSnake = turningVectors[newDirectionIndex];
		m_xyzDirectionOfSnake = m_xzDirectionOfSnake;

		for (auto& iterator : m_cubes)
			iterator->ToggleChangingDirection(m_xyzDirectionOfSnake, m_topRightChangingPoints[newDirectionIndex]);
		++newDirectionIndex;
		otherDirectionIndex = 4 - newDirectionIndex;
		if (newDirectionIndex == 4)
			newDirectionIndex = 0;
		m_isMovingInAltitude = false;
	}
	void ChangeToNewDirectionDU(float yDirection)
	{
		m_xyzDirectionOfSnake = glm::vec3(0.0f, yDirection, 0.0f);

		for (auto& iterator : m_cubes)
			iterator->ToggleChangingDirection(m_xyzDirectionOfSnake, m_topRightChangingPoints[m_indexRightVectors]);

		m_isMovingInAltitude = true;
	}
	void MoveRight(void)
	{
		CreateRightChangingPoint();
		ChangeToNewDirectionLR(m_indexRightVectors, m_indexLeftVectors, m_turningRightVectors);
	}
	void MoveLeft(void)
	{
		CreateLeftChangingPoint();
		ChangeToNewDirectionLR(m_indexLeftVectors, m_indexRightVectors, m_turningLeftVectors);
	}
	void MoveUp(void)
	{
		CreateRightChangingPoint();
		ChangeToNewDirectionDU(1.0f);
	}
	void MoveDown(void)
	{
		CreateRightChangingPoint();
		ChangeToNewDirectionDU(-1.0f);
	}
	const bool Vect3HasNegativeNumber(glm::vec3 vec, unsigned int* index)
	{
		if (vec.x < 0.0f || vec.y < 0.0f || vec.z < 0.0f)
			return true;
		return false;
	}
	void CreateChangingPoint(float zExt1, float zExt2)
	{
		m_topRightChangingPoints[0] = glm::vec3(ceil(m_cubes[0]->ShapeVerts()->m_right),
			m_cubes[0]->ShapeVerts()->m_top,
			m_cubes[0]->ShapeVerts()->m_front);
		m_topRightChangingPoints[1] = glm::vec3(m_cubes[0]->ShapeVerts()->m_right,
			m_cubes[0]->ShapeVerts()->m_top,
			zExt1);
		m_topRightChangingPoints[2] = glm::vec3(floor(m_cubes[0]->ShapeVerts()->m_right),
			m_cubes[0]->ShapeVerts()->m_top,
			m_cubes[0]->ShapeVerts()->m_front);
		m_topRightChangingPoints[3] = glm::vec3(m_cubes[0]->ShapeVerts()->m_right,
			m_cubes[0]->ShapeVerts()->m_top,
			zExt2);
	}
	void ChangeChangingPointYToExtremityOfY(void)
	{
		for (unsigned short iter = 0; iter < 4; ++iter)
		{
			float yVal;
			if (m_xyzDirectionOfSnake.y > 0.1f)
				yVal = ceil(Head()->ShapeVerts()->m_top);
			else
				yVal = floor(Head()->ShapeVerts()->m_top);

			m_topRightChangingPoints[iter].y = yVal;
		}
	}
	void CreateRightChangingPoint(void)
	{
		CreateChangingPoint(ceil(m_cubes[0]->ShapeVerts()->m_front),
			floor(m_cubes[0]->ShapeVerts()->m_front));

		if (m_isMovingInAltitude)
			ChangeChangingPointYToExtremityOfY();
	}
	void CreateLeftChangingPoint(void)
	{
		CreateChangingPoint(floor(m_cubes[0]->ShapeVerts()->m_front),
			ceil(m_cubes[0]->ShapeVerts()->m_front));

		if (m_isMovingInAltitude)
			ChangeChangingPointYToExtremityOfY();
	}
	void SetAppleCollisionVectorToAllGreaterCoords(Shape::ShapeVertices* shapeVertsOfApple)
	{
		m_appleCollisionVector = glm::vec3(shapeVertsOfApple->m_right,
			shapeVertsOfApple->m_top, shapeVertsOfApple->m_front);
	}
	void SetAppleCollisionVectorToAllLesserCoords(Shape::ShapeVertices* shapeVertsOfApple)
	{
		m_appleCollisionVector = glm::vec3(shapeVertsOfApple->m_left,
			shapeVertsOfApple->m_bottom, shapeVertsOfApple->m_back);
	}
	void GetCompareVector(Apple* apple)
	{
		Shape::ShapeVertices* shapeVertsOfApple = apple->CubeObj()->ShapeVerts();
		Shape::ShapeVertices* shapeVertsOfSnakeHead = Head()->ShapeVerts();

		if (fabs(m_xyzDirectionOfSnake.x) > 0.1f)
		{
			// collision vector with .left or .right of shape vertices

			if (m_xyzDirectionOfSnake.x < -0.1f)
			{
				// direction of snake.x is negative
				// compare with snake.left 
				// and apple.right

				SetAppleCollisionVectorToAllGreaterCoords(shapeVertsOfApple);
				m_snakeCollisionVector = { &shapeVertsOfSnakeHead->m_left,
					&shapeVertsOfSnakeHead->m_top, &shapeVertsOfSnakeHead->m_front };
			}
			else if (m_xyzDirectionOfSnake.x > 0.1f)
			{
				// direction of snake.x is positive
				// compare with snake.right
				// and apple.left

				SetAppleCollisionVectorToAllLesserCoords(shapeVertsOfApple);
				m_snakeCollisionVector = { &shapeVertsOfSnakeHead->m_right,
					&shapeVertsOfSnakeHead->m_bottom, &shapeVertsOfSnakeHead->m_back };
			}
		}
		if (fabs(m_xyzDirectionOfSnake.y) > 0.1f)
		{
			// collision vector with .bottom or .top of shape vertices

			if (m_xyzDirectionOfSnake.y < -0.1f)
			{
				SetAppleCollisionVectorToAllGreaterCoords(shapeVertsOfApple);
				m_snakeCollisionVector = { &shapeVertsOfSnakeHead->m_right,
					&shapeVertsOfSnakeHead->m_bottom, &shapeVertsOfSnakeHead->m_front };
			}
			else if (m_xyzDirectionOfSnake.y > 0.1f)
			{
				SetAppleCollisionVectorToAllLesserCoords(shapeVertsOfApple);
				m_snakeCollisionVector = { &shapeVertsOfSnakeHead->m_left,
					&shapeVertsOfSnakeHead->m_top, &shapeVertsOfSnakeHead->m_back };
			}
		}
		if (fabs(m_xyzDirectionOfSnake.z) > 0.1f)
		{
			// collision vector with .back or .fron of shape vertices

			if (m_xyzDirectionOfSnake.z < -0.1f)
			{
				SetAppleCollisionVectorToAllGreaterCoords(shapeVertsOfApple);
				m_snakeCollisionVector = { &shapeVertsOfSnakeHead->m_right,
					&shapeVertsOfSnakeHead->m_top, &shapeVertsOfSnakeHead->m_back };
			}
			else if (m_xyzDirectionOfSnake.z > 0.1f)
			{
				SetAppleCollisionVectorToAllLesserCoords(shapeVertsOfApple);
				m_snakeCollisionVector = { &shapeVertsOfSnakeHead->m_left,
					&shapeVertsOfSnakeHead->m_bottom, &shapeVertsOfSnakeHead->m_front };
			}
		}
	}
private:
	void InitializeRightTurningVectors(void)
	{
		m_turningRightVectors[0] = glm::vec3(0.0f, 0.0f, 1.0f);
		m_turningRightVectors[1] = glm::vec3(-1.0f, 0.0f, 0.0f);
		m_turningRightVectors[2] = glm::vec3(0.0f, 0.0f, -1.0f);
		m_turningRightVectors[3] = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	void InitializeLeftTurningVectors(void)
	{
		m_turningLeftVectors[0] = glm::vec3(0.0f, 0.0f, -1.0f);
		m_turningLeftVectors[1] = glm::vec3(-1.0f, 0.0f, 0.0f);
		m_turningLeftVectors[2] = glm::vec3(0.0f, 0.0f, 1.0f);
		m_turningLeftVectors[3] = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	void InitializeAppleAndSnakeCollisionVectors(Shape::ShapeVertices* shapeVertsOfApple,
		Shape::ShapeVertices* shapeVertsOfSnakeHead)
	{
		m_appleCollisionVector = glm::vec3(shapeVertsOfApple->m_left,
			shapeVertsOfApple->m_bottom, shapeVertsOfApple->m_back);
		m_snakeCollisionVector = { &shapeVertsOfSnakeHead->m_right,
			&shapeVertsOfSnakeHead->m_bottom, &shapeVertsOfSnakeHead->m_back };
	}
	void Init(Apple* apple)
	{
		// the head of the snake
		m_cubes.push_back(new Cube(GREEN, 0.5f, m_xzDirectionOfSnake, glm::vec3(0.0f, 0.0f, -10.0f), m_speed, std::vector<Shape::Movement>()));
		for (unsigned int i = 0; i < 3; ++i)
			AddCube();

		InitializeRightTurningVectors();
		InitializeLeftTurningVectors();
		// the beginning of the game the direction of the snake
		// is always glm::vec3(1.0f, 0.0f, 0.0f);

		Shape::ShapeVertices* shapeVertsOfApple = apple->CubeObj()->ShapeVerts();
		Shape::ShapeVertices* shapeVertsOfSnakeHead = Head()->ShapeVerts();

		InitializeAppleAndSnakeCollisionVectors(shapeVertsOfApple, shapeVertsOfSnakeHead);
	}
	void SnakeAteApple(Apple* apple)
	{
		glm::vec3 currentCompareVecPositionOfSnakeHead = glm::vec3(
			*m_snakeCollisionVector.m_x,
			*m_snakeCollisionVector.m_y,
			*m_snakeCollisionVector.m_z
		);
		if (glm::all(glm::lessThan(glm::abs(currentCompareVecPositionOfSnakeHead - m_appleCollisionVector), 
			glm::vec3(0.1f))))
		{
			apple->GenerateNewApple();
			AddCube();

			apple->CubeObj()->UpdateShapeVertices();
			GetCompareVector(apple);
		}
	}
	const bool Vec3HasAllPos(glm::vec3 vec)
	{
		if (vec.x > 0.0f &&
			vec.y > 0.0f &&
			vec.z > 0.0f)
		{
			return true;
		}
		return false;
	}
	const bool SurpassedLimit(void)
	{
		if (m_cubes[0]->ShapeVerts()->m_top > m_radiusOfGrid.y ||
			fabs(m_cubes[0]->ShapeVerts()->m_bottom) > m_radiusOfGrid.y ||
			m_cubes[0]->ShapeVerts()->m_right > m_radiusOfGrid.x ||
			fabs(m_cubes[0]->ShapeVerts()->m_left) > m_radiusOfGrid.x ||
			m_cubes[0]->ShapeVerts()->m_front > m_radiusOfGrid.z ||
			fabs(m_cubes[0]->ShapeVerts()->m_front > m_radiusOfGrid.z))
		{
			return true;
		}
	}
private:
	std::vector<Shape*> m_cubes;

	glm::vec3 m_xzDirectionOfSnake;			//if the snake is going up, the direction of the snake before is saved here
	glm::vec3 m_xyzDirectionOfSnake;		

	glm::vec3 m_colorDelta;
	const unsigned short m_headCube = 0;
	bool m_isChangingDirection;
	glm::vec3 m_topRightOfCubesChangingPoint;

	glm::vec3 m_turningRightVectors[4];
	glm::vec3 m_turningLeftVectors[4];
	glm::vec3 m_topRightChangingPoints[4];
	unsigned short m_indexRightVectors;
	unsigned short m_indexLeftVectors;

	bool m_isMovingInAltitude;
	float m_speed;

	glm::vec3 m_radiusOfGrid;

	glm::vec3 m_appleCollisionVector;
	Snake::LiveVector m_snakeCollisionVector;
	unsigned int m_currentBaseColor;
};

#endif