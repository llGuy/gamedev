#ifndef GAME_HEADER
#define GAME_HEADER

#include <ctime>

#include "..\shape\line.h"
#include "..\grid\grid.h"
#include "..\camera\camera.h"
#include "..\shader\shprogram.h"
#include "..\entities\snake.h"
#include "..\entities\apple.h"

class Game
{
public:
	explicit Game(unsigned int width, unsigned int height)
		: m_shprogram("res\\vsh.shader",
			"res\\fsh.shader"), m_windowWidth(width),
		  m_windowHeight(height), m_centerOfGrid(-10.0f), m_lost(false)
	{
	}
public:
	void Init(void)
	{
		m_shprogram.Compile();
		m_shprogram.Link();

		m_grid = new Grid(10.0f, -10.0f);
		m_apple = new Apple();
		m_snake = new Snake(0.01f, 10.0f, m_apple);
		m_reference = new Cube(RED, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -10.0f), 0.002f, std::vector<Shape::Movement>());
	}
public:
	void Draw(Camera* camera)
	{ 
		unsigned int location = glGetUniformLocation(m_shprogram.ProgramID(), "transformMatrix");

		m_viewProjectionMatrix = glm::perspective(
			glm::radians(60.0f), (float)m_windowWidth / m_windowHeight, 0.1f, 40.0f) *
			camera->GetWorldToViewMatrix(m_snake, m_grid);

		m_snake->Draw(m_viewProjectionMatrix, location, m_apple, &m_lost);
		m_reference->Draw(m_viewProjectionMatrix, location);
		m_grid->Draw(m_viewProjectionMatrix, location);
		m_apple->Draw(m_viewProjectionMatrix, location);

		if (m_lost)
		{
			std::cout << "GAME OVER\n";
		}
	}
	void MoveSnake(Snake::movement_t movement)
	{
		m_snake->Move(movement, m_apple);
	}
	Snake* SnakeObj(void)
	{
		return m_snake;
	}
	bool GameOver(void)
	{
		return m_lost;
	}
private:
	SHProgram m_shprogram;
	
	Grid* m_grid;
	Snake* m_snake;
	Shape* m_reference;
	Apple* m_apple;

	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	float m_centerOfGrid;

	glm::mat4 m_viewProjectionMatrix;
	float m_gridRadius;
	bool m_lost;
};

#endif