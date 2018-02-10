#include "robot.h"

#include <GL/glew.h>

#include "../primitives/vertex.h"
#include "../engine.h"

#include "../data/time.h"
#include "../bullet/gun.h"

Robot::Robot(float radius, glm::vec2 plainPosition, glm::vec3 color)
	: m_cubeRadius(radius), m_troopProximity(40.0f), m_lockedOnTroop(false), m_lockedTroop(nullptr)
{
	m_cube = new Cube(m_cubeRadius, color, true);
	m_translateVectorPlainPosition = plainPosition;
	m_gun = new Gun(0.1f);
	m_cube->Init();
	RobotDataInit();
}
void Robot::SendUniformData(Entity::UniData& ud, glm::mat4& model, UniformLocations* locations, Time* time)
{
	glUniformMatrix4fv(locations->m_uniLocProjection, 1, GL_FALSE, &ud.projection[0][0]);
	glUniformMatrix4fv(locations->m_uniLocView, 1, GL_FALSE, &ud.view[0][0]);
	glUniformMatrix4fv(locations->m_uniLocModel, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(locations->m_uniLocLightPosition, 1, &ud.lightPosition[0]);
	glUniform3fv(locations->m_uniLocEyePosition, 1, &ud.eyePosition[0]);
	glUniform1f(locations->m_uniLocTime, (float)(time->currentTime - time->beginning).count() / 1000000000);
}
void Robot::RobotDataInit(void)
{
	m_lives = static_cast<unsigned int>(m_cubeRadius * 4);
	m_robotSpeed = 0.007f / m_cubeRadius;
	m_viewDirection = glm::vec2(1.0f, 0.0f);
	m_circleRadius = glm::distance(glm::vec3(0.0f), glm::vec3(+m_cubeRadius, +m_cubeRadius, +m_cubeRadius));
}
const bool Robot::WantsToShoot(void)
{
	return rand() % 511 == 3;
}
void Robot::Shoot(glm::vec3 playerPosition)
{
	if (!m_lockedOnTroop)
	{
		m_gun->Shoot(glm::normalize(playerPosition - m_worldCoordinates),
			m_worldCoordinates + glm::vec3(0.0f, 0.5f, 0.0f));
	}
	else
	{
		if (TroopAlive())
		{
			glm::vec3 direction = glm::normalize(m_lockedTroop->Position() - m_worldCoordinates);
			m_gun->Shoot(direction, m_worldCoordinates + glm::vec3(0.0f, 0.5f, 0.0f));
		}
		else
		{
			m_lockedOnTroop = false;
			m_lockedTroop = nullptr;
		}
	}
}
const bool Robot::TroopAlive(void)
{
	return m_lockedTroop->Alive();
}

// new

const bool Robot::Draw(Entity::UniData& ud, UniformLocations* locations,
	Entity::DrawData& dd, Entity* player, Entity::Troops_t& troops)
{
	m_hitPlayer = false;
	m_cube->Draw(ud, m_translateMatrix, locations, dd.timeData);
	if (WantsToShoot()) Shoot(ud.eyePosition);
	if (m_gun->BulletAiring())
	{
		if(!m_lockedOnTroop)
			m_hitPlayer = m_gun->Draw(ud, locations, dd, player);
		else
		{
			if (TroopAlive()) m_gun->Draw(ud, locations, dd, troops);
		}
	}
	return m_hitPlayer;
}
void Robot::Move(const Entity::move_t&& movement, const glm::vec2& playerPlainPos)
{
	if (movement == Entity::move_t::TO_PLAYER)
	{
		if(!m_lockedOnTroop)
		{
			glm::vec2 dir = glm::normalize(playerPlainPos - m_translateVectorPlainPosition);
			m_translateVectorPlainPosition += dir * m_robotSpeed;
			m_translateMatrix = glm::translate(m_worldCoordinates);
		}
		else
		{
			glm::vec2 dir = glm::normalize(m_lockedTroop->Position() - m_worldCoordinates);
			m_translateVectorPlainPosition += dir * m_robotSpeed;
			m_translateMatrix = glm::translate(m_worldCoordinates);
		}
	}
	else return;
}
const bool Robot::DetectBulletCollision(const glm::vec3& worldCoords,
	const float& circleRad)
{
	return glm::distance(m_worldCoordinates, worldCoords) < circleRad + m_circleRadius;
}
const bool Robot::Alive(void)
{
	return m_lives > 0;
}
void Robot::DeleteBuffers(void)
{
	m_cube->DeleteBuffers();
}
void Robot::RemoveLife(void)
{
	--m_lives;
}
void Robot::UpdTransMat(const float& terrHeight)
{
	m_worldCoordinates = glm::vec3(m_translateVectorPlainPosition.x, terrHeight, m_translateVectorPlainPosition.y);
	m_translateMatrix = glm::translate(m_worldCoordinates);
}
glm::vec2 Robot::PlainPosition(void)
{
	return glm::vec2(m_translateVectorPlainPosition.x,
		m_translateVectorPlainPosition.y);
}
void Robot::Power(const power_t&& power, const glm::vec3& playerPos)
{
	if (power == Entity::power_t::SHOOT) Shoot(playerPos);
	else return;
}
glm::vec3 Robot::Position(void)
{
	return m_worldCoordinates;
}
void Robot::FindClosestTroop(Troops_t& troops)
{
	for (auto& i : troops)
	{
		int wantsToLockOnTroop = rand() % 23;
		if (wantsToLockOnTroop == 3 && 
			glm::distance(i->PlainPosition(), this->PlainPosition()) < m_troopProximity && 
			!m_lockedOnTroop)
		{
			m_lockedOnTroop = true;
			m_viewDirection = i->PlainPosition();
			m_lockedTroop = i;

			break;
		}
	}
}