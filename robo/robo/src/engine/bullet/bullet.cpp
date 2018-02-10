#include <GL/glew.h>
#include <math.h>

#include "bullet.h"
#include "../entities/entity.h"
#include "../engine.h"

Bullet::Bullet(glm::vec3 playerViewDirection,
	glm::vec3 playerEyePos, float bulletSpeed, glm::vec3 color)
	: m_cubeRadius(0.1f), m_bulletSpeed(bulletSpeed), m_color(color)
{
	m_cube = new Cube(m_cubeRadius, m_color, false);
	m_cube->Init();
	Init(playerViewDirection, playerEyePos);
	BulletDataInit();
}
Bullet::~Bullet(void)
{
}
void Bullet::DeleteBuffer(void)
{
	m_cube->DeleteBuffers();
}
void Bullet::Draw(Entity::UniData& ud, UniformLocations* locations, Time* time)
{
	m_cube->Draw(ud, m_translateMatrix, locations, time);
}
glm::vec2 Bullet::PlainPosition(void)
{
	return m_translateVectorPlainPosition;
}
void Bullet::Move(void)
{
	m_worldCoordinates += m_direction * m_bulletSpeed;
}
void Bullet::UpdateTranslateMatrix(void)
{
	m_translateMatrix = glm::translate(m_worldCoordinates);
	m_translateVectorPlainPosition = glm::vec2(m_worldCoordinates.x, m_worldCoordinates.z);
}
const bool Bullet::CollisionCheck(float heightOfTerrain, std::vector<Entity*>& vec)
{
	for (unsigned short i = 0; i < vec.size(); ++i)
		if (vec[i]->DetectBulletCollision(m_worldCoordinates, m_circleRadius))
		{
			vec[i]->RemoveLife();
			if (!vec[i]->Alive())
			{
				vec[i]->DeleteBuffers();
				vec.erase(vec.begin() + i);
			}
			return true;
		}
	return false;
}
const bool Bullet::CollisionCheck(float heightOfTerrain, Entity* player)
{
	if (glm::all(glm::lessThan(glm::abs(player->Position() - m_worldCoordinates), glm::vec3(0.5f))))
	{
		player->RemoveLife();
		return true;
	}
	return false;
}
const bool Bullet::CheckTerrainCollision(float heightOfTerrain)
{
	if (fabs(heightOfTerrain - m_worldCoordinates.y) < 0.01f || heightOfTerrain > m_worldCoordinates.y)
		return true;
	return false;
}
void Bullet::Init(glm::vec3& playerViewDirection,
	glm::vec3 playerEyePos)
{
	m_direction = playerViewDirection;
	m_worldCoordinates = playerEyePos;
	m_translateVectorPlainPosition = glm::vec2(m_worldCoordinates.x,
		m_worldCoordinates.z);
}  
void Bullet::BulletDataInit(void)
{
	m_circleRadius = glm::distance(glm::vec3(0.0f), glm::vec3(+m_cubeRadius, +m_cubeRadius, +m_cubeRadius));
}
const bool Bullet::EscapedTerrainLimits(float x, float z)
{
	return (fabs(m_worldCoordinates.x) > x / 2) || (fabs(m_worldCoordinates.z) > z / 2);
}
const float& Bullet::CircleRadius(void)
{
	return m_circleRadius;
}
glm::vec3 Bullet::WorldCoords(void)
{
	return m_worldCoordinates;
}