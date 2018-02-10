#ifndef GAME_HEADER
#define GAME_HEADER

#include "..\shader\shprogram.h"
#include "..\shape\ball.h"
#include "..\camera\camera.h"
#include "..\shape\paddle.h"
#include "..\shape\line.h"

#include <time.h>
#include <glm\gtx\transform.hpp>
#include <vector>

class Game
{
public:
	explicit Game(void)
		: m_program("res\\vsh.shader",
			"res\\fsh.shader")
	{
	}
public:
	void Init(unsigned int width, unsigned int height)
	{	
		m_balls.push_back(new Ball(YELLOW, 
			glm::vec3(0.0f, 0.0f, m_zValueOfAllCloseVerts), glm::vec3(1.0f, -0.7f, 0.0f),
			m_topGamePlane, m_bottomGamePlane));

		m_paddles.push_back(new Paddle(RED, glm::vec3(m_xValPaddleLeft, 0.0f, m_zValueOfAllCloseVerts)));
		m_paddles.push_back(new Paddle(BLUE, glm::vec3(m_xValPaddleRight, 0.0f, m_zValueOfAllCloseVerts)));
		
		m_program.Compile();
		m_program.Link();
	}
	void AddBall(void)
	{
		Color colorOfNewBall = GenerateColor();
		m_balls.push_back(new Ball(colorOfNewBall, 
			glm::vec3(0.0f, 0.0f, m_zValueOfAllCloseVerts), glm::vec3(1.0f, -0.7f, 0.0f), 
			m_topGamePlane, m_bottomGamePlane));
	}
public:
	void Draw(unsigned int width, unsigned int height, Camera* camera)
	{
		unsigned int location = glGetUniformLocation(m_program.ProgramID(), "transformMatrix");
		
		m_worldToViewProjectionMatrix =
			glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 40.0f) *
			camera->GetWorldToViewMatrix();

		for (auto& ballIterator : m_balls)
			ballIterator->Draw(m_worldToViewProjectionMatrix, location, 
				&m_paddles[0]->ShapeVerts(), &m_paddles[1]->ShapeVerts());

		for (auto& paddleIterator : m_paddles)
			paddleIterator->Draw(m_worldToViewProjectionMatrix, location);
	}
	Shape*& PaddleLeft(void)
	{
		return m_paddles[0];
	}
	Shape*& PaddleRight(void)
	{
		return m_paddles[1];
	}
private:
	color_t GenerateColor(void)
	{
		srand(time(NULL));
		unsigned int colorUInt = std::rand() % 5;
	
		color_t finalColor = static_cast<color_t>(colorUInt);
		return finalColor;
	}
private:
	SHProgram m_program;

	std::vector<Shape*> m_balls;
	std::vector<Shape*> m_paddles;
	
	glm::mat4 m_worldToViewProjectionMatrix;

	const float m_zValueOfAllCloseVerts = -7.0f;
	const float m_xValPaddleLeft		= -4.0f;
	const float m_xValPaddleRight		= +4.0f;
	const float m_topGamePlane			= +3.399f;
	const float m_bottomGamePlane		= -3.7f;
};

#endif