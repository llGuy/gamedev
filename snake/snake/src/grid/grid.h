#ifndef GRID_HEADER
#define GRID_HEADER

#include <vector>

#include "..\shape\line.h"

class Grid
{
public:
	explicit Grid(float radius, float center)
		: m_radius(radius), m_center(center)
	{
		Init();
	}
public:
	void Draw(glm::mat4 projectionMatrix, unsigned int location)
	{
		m_x->Draw(projectionMatrix, location);
		m_y->Draw(projectionMatrix, location);
		m_z->Draw(projectionMatrix, location);

		glDrawElements(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT, 0);
	}
	glm::vec3 Center(void)
	{
		return glm::vec3(0.0f, 0.0f, m_center);
	}
private:
	void Init(void)
	{
		m_x = new Line(glm::vec3(-m_radius, 0.0f, 0.0f), 
			glm::vec3(m_radius, 0.0f, 0.0f), 
			glm::vec3(0.0f, 0.0f, m_center), YELLOW);

		m_y = new Line(glm::vec3(0.0f, -m_radius, 0.0f),
			glm::vec3(0.0f, m_radius, 0.0f),
			glm::vec3(0.0f, 0.0f, m_center), GREEN);

		m_z = new Line(glm::vec3(0.0f, 0.0f, -m_radius),
			glm::vec3(0.0f, 0.0f, m_radius),
			glm::vec3(0.0f, 0.0f, m_center), RED);
	}
private:
	Shape* m_x;
	Shape* m_y;
	Shape* m_z;

	float m_radius;
	float m_center;
};

#endif