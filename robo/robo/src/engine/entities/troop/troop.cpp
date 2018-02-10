#include "troop.h"
#include "../../engine.h"
#include "../../bullet/gun.h"

Troop::Troop(float radius, glm::vec3 plainPosition,
	glm::vec3 color)
	: m_lives(20), m_yElevation(1.0f), m_cubeRadius(radius)
{
	m_worldCoordinates = plainPosition + glm::vec3(0.0f, m_yElevation, 0.0f);
	m_cube = new Cube(radius, color, false);
	m_circleRadius = glm::distance(glm::vec3(0.0f), glm::vec3(+m_cubeRadius, +m_cubeRadius, +m_cubeRadius));
	m_cube->Init();
	m_gun = new Gun();
	TroopDataInit();
}

const bool Troop::Draw(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, Entity::Robots_t& robots)
{
	m_cube->Draw(ud, m_translateMatrix, locations, dd.timeData);
	if (WantsToShoot())
		Shoot(robots);

	return true;
}
const bool Troop::BulletAiring(void)
{
	return m_gun->BulletAiring();
}
const bool Troop::DetectBulletCollision(const glm::vec3& worldCoords,
	const float& circleRad)
{
	return glm::distance(m_worldCoordinates, worldCoords) < circleRad + m_circleRadius;
}
glm::vec3 Troop::Position(void)
{
	return m_worldCoordinates;
}
const bool Troop::Alive(void)
{
	return m_lives > 0;
}
void Troop::DeleteBuffers(void)
{
	m_cube->DeleteBuffers();
}
void Troop::RemoveLife(void)
{
	--m_lives;
}
glm::vec2 Troop::PlainPosition(void)
{
	return glm::vec2(m_worldCoordinates.x, m_worldCoordinates.z);
}
void Troop::Power(const power_t&& power, const std::vector<Entity*>& robots)
{
	if (power == Entity::power_t::SHOOT)
		if (WantsToShoot()) 
			Shoot(robots);
}
void Troop::DrawBullets(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, Entity::Robots_t& vec)
{
	m_gun->Draw(ud, locations, dd, vec);
}
void Troop::SendUniformData(Entity::UniData& ud, glm::mat4& model, UniformLocations* locations, Time* time)
{
	glUniformMatrix4fv(locations->m_uniLocProjection, 1, GL_FALSE, &ud.projection[0][0]);
	glUniformMatrix4fv(locations->m_uniLocView, 1, GL_FALSE, &ud.view[0][0]);
	glUniformMatrix4fv(locations->m_uniLocModel, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(locations->m_uniLocLightPosition, 1, &ud.lightPosition[0]);
	glUniform3fv(locations->m_uniLocEyePosition, 1, &ud.eyePosition[0]);
	glUniform1f(locations->m_uniLocTime, (float)(time->currentTime - time->beginning).count() / 1000000000);
}
void Troop::TroopDataInit(void)
{
	m_translateMatrix = glm::translate(m_worldCoordinates);
}
const bool Troop::WantsToShoot(void)
{
	int random = rand() % 25;
	return random == 13;
}
void Troop::Shoot(const std::vector<Entity*>& robots)
{
	for (auto& i : robots)
	{
		if (WantsToShoot())
		{
			glm::vec3 direction = glm::normalize(i->Position() - m_worldCoordinates);
			m_gun->Shoot(direction, m_worldCoordinates);
		}
	}
}