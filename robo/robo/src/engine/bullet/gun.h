#ifndef GUN_HEADER
#define GUN_HEADER

#include <vector>

#include "bullet.h"
#include "../terrain/terrain.h"
#include "../robot/robot.h"
#include "../entities/entity.h"

class Gun
{
public:
	explicit Gun(float bulletSpeed = 0.3f)
		: m_bullets(), m_bulletSpeed(bulletSpeed)
	{
	}

	void Shoot(glm::vec3 playerViewDirection, glm::vec3 playerEyePos, glm::vec3 color = glm::vec3(0.7f))
	{ 
		m_bullets.push_back(Bullet(playerViewDirection, playerEyePos, m_bulletSpeed, color));
	}
	// draw function for the player
	void Draw(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, std::vector<Entity*>& vec)
	{
		MoveBullets(ud, locations, dd, vec);
		for (auto& i : m_bullets)
			i.Draw(ud, locations, dd.timeData);
	}
	// draw function for the robots
	const bool Draw(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, Entity* player)
	{
		bool hitPlayer = MoveBullets(ud, locations, dd, player);
		for (auto& i : m_bullets)
			i.Draw(ud, locations, dd.timeData);
		return hitPlayer;
	}
	const bool BulletAiring(void)
	{
		return m_bullets.size() > 0;
	}
private:
	const bool MoveBullets(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, Entity::Player_t player)
	{
		for (unsigned short i = 0; i < m_bullets.size(); ++i)
		{
			if (m_bullets[i].CollisionCheck(dd.terrain->GetYPosOfPlayer(m_bullets[i].PlainPosition().x, 
				m_bullets[i].PlainPosition().y), player))
			{
				m_bullets[i].DeleteBuffer();
				m_bullets.erase(m_bullets.begin() + i);
				return true;
			}
			else if (m_bullets[i].CheckTerrainCollision(dd.terrain->GetYPosOfPlayer(m_bullets[i].PlainPosition().x, 
				m_bullets[i].PlainPosition().y)))
			{
				m_bullets[i].DeleteBuffer();
				m_bullets.erase(m_bullets.begin() + i);
			}
			else if (m_bullets[i].EscapedTerrainLimits(dd.terrain->Dimensions().x, dd.terrain->Dimensions().y))
			{
				m_bullets[i].DeleteBuffer();
				m_bullets.erase(m_bullets.begin() + i);
			}
			else
			{
				m_bullets[i].Move();
				m_bullets[i].UpdateTranslateMatrix();
			}
		}
		return false;
	}
	void MoveBullets(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, std::vector<Entity*>& robots)
	{
		for (unsigned short i = 0; i < m_bullets.size(); ++i)
		{
			if (m_bullets[i].CollisionCheck(dd.terrain->GetYPosOfPlayer(m_bullets[i].PlainPosition().x, m_bullets[i].PlainPosition().y), robots) ||
				m_bullets[i].EscapedTerrainLimits(dd.terrain->Dimensions().x, dd.terrain->Dimensions().y))
			{
				m_bullets[i].DeleteBuffer();
				m_bullets.erase(m_bullets.begin() + i);
			}
			else
			{
				m_bullets[i].Move();
				m_bullets[i].UpdateTranslateMatrix();
			}
		}
	}
private:
	std::vector<Bullet> m_bullets;
	float m_bulletSpeed;
};

#endif