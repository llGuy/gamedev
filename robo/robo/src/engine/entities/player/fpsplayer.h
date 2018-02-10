#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "../../bullet/gun.h"
#include "../../robot/robot.h"
#include "../troop/troop.h"
#include "../entity.h"

class FPSPlayer final
	: public Entity
{
public:
	struct FPSPlayerData
	{
		glm::vec3 position;
		glm::vec3 viewDirection;
		float speed;
		float height;
		float viewBobbingValue;
		float runningDelta;
	};
	struct BoostData
	{
		glm::vec2 viewDirection;
		glm::vec2 resistance;
		glm::vec2 velocity;
		bool boosting;

		bool allowedToBoost;
	};
	struct JumpData
	{
		glm::vec3 velocity;
		glm::vec3 gravity;
		bool jumping;
	};
	struct FlourishData
	{
		glm::vec3 velocity;
		glm::vec3 gravity;
		bool flourishing;
		bool disabled;
		float yInclination;
		float angle;
		unsigned int colorIndex;

		glm::vec3 rainbowColors[7] = 
		{
			glm::vec3(148.0f / 255.0f, 0.0f, 211.0f / 255.0f) * 0.6f,
			glm::vec3(75.0f / 255.0f, 0.0f, 130 / 255.0f) * 0.6f,
			glm::vec3(0.0f, 0.0f, 1.0f) * 0.6f,
			glm::vec3(0.0f, 1.0f, 0.0f) * 0.6f,
			glm::vec3(1.0f, 1.0f, 0.0f) * 0.6f,
			glm::vec3(1.0f, 0.5f, 0.0f) * 0.6f,
			glm::vec3(1.0f, 0.0f, 0.0f) * 0.6f
		};
	};
public:
	explicit FPSPlayer(FPSPlayerData pData)
		:	m_pData(pData),
			m_up(0.0f, 1.0f, 0.0f),
			m_gun()
	{
		m_pData.position.y += m_pData.height;
		m_viewBobbing = pData.viewBobbingValue;
		m_viewBobbingDelta = m_viewBobbing;
		m_runningDelta = pData.runningDelta;
		m_jd.jumping = false;
		m_fd.flourishing = false;
		m_bd.allowedToBoost = true;

		m_lives = 5;
	}
public:
	// getter methods
	const bool PlacedTroops(void) override
	{
		return m_troop.size() > 0;
	}
	Troops_t* Troops(void) override
	{
		return &m_troop;
	}
	glm::vec3 Position(void) override
	{
		return m_pData.position;
	}
	glm::vec3* ViewDirection(void) override
	{
		return &m_pData.viewDirection;
	}
	void UpdData(Terrain* terrain, Time* time) override
	{
		if (m_fd.flourishing)
		{
			Flourish(static_cast<float>(time->deltaT));
			CheckFlourishLanding(terrain->GetYPosOfPlayer(m_pData.position.x, m_pData.position.z));
		}
		if (m_jd.jumping)
		{
			Jump(static_cast<float>(time->deltaT));
			CheckJumpLanding(terrain->GetYPosOfPlayer(m_pData.position.x, m_pData.position.z));
		}
		if (m_bd.boosting)
		{
			Boost(static_cast<float>(time->deltaT));
			m_pData.position.y = terrain->GetYPosOfPlayer(m_pData.position.x, m_pData.position.z) + m_pData.height;
			CheckBoostEnd();
		}
	}
public:
	void ViewBobbing(void)
	{
		if (!m_jd.jumping && !m_fd.flourishing)
		{
			if (m_running) m_viewBobbing += m_viewBobbingDelta * 1.5f;
			else m_viewBobbing += m_viewBobbingDelta;
			if (m_pData.position.y + m_viewBobbing > m_pData.position.y + 0.1f ||
				m_pData.position.y + m_viewBobbing < m_pData.position.y)
				m_viewBobbingDelta *= -1.0f;

			m_pData.position.y += m_viewBobbing;
		}
	}
	void Move(const move_t&& movement, const float& terrHeight,
		const glm::vec3& direction = glm::vec3(0.0f)) override
	{
		if (movement == Entity::move_t::FORWARD)
		{
			glm::vec3 toMoveVector = glm::vec3(m_pData.viewDirection.x, 0.0f, m_pData.viewDirection.z);
			m_pData.position += toMoveVector * m_pData.speed;
		}
		else if (movement == Entity::move_t::BACKWARD)
		{
			glm::vec3 toMoveVector = glm::vec3(m_pData.viewDirection.x, 0.0f, m_pData.viewDirection.z);
			m_pData.position -= toMoveVector * m_pData.speed;
		}
		if(!m_jd.jumping && !m_fd.flourishing)
			m_pData.position.y = terrHeight + m_pData.height;
	}
	void Strafe(const strafe_t&& strafe, const float& terrHeight) override
	{
		if (strafe == Entity::strafe_t::RIGHT)
		{
			glm::vec3 strafeDirection = glm::cross(m_pData.viewDirection, m_up);
			m_pData.position += strafeDirection * m_pData.speed;
		}
		else if (strafe == Entity::strafe_t::LEFT)
		{
			glm::vec3 strafeDirection = glm::cross(m_pData.viewDirection, m_up);
			m_pData.position -= strafeDirection * m_pData.speed;
		}
		if(!m_jd.jumping && !m_fd.flourishing)
			m_pData.position.y = terrHeight + m_pData.height;
	}
	void Look(glm::vec2 newMousePosition)
	{
		glm::vec2 mouseDelta = newMousePosition - m_oldMousePosition;
		m_pData.viewDirection = glm::mat3(glm::rotate(glm::radians(-mouseDelta.x) * 0.02f, m_up)) * m_pData.viewDirection;
		glm::vec3 toRotateAround = glm::cross(m_pData.viewDirection, m_up);
		m_pData.viewDirection = glm::mat3(glm::rotate(glm::radians(-mouseDelta.y) * 0.02f, toRotateAround)) * m_pData.viewDirection;
		m_oldMousePosition = newMousePosition;
	}
public:
	void Power(const power_t&& power, 
		const glm::vec3& playerPos = glm::vec3(0.0f)) override 
	{
		if (power == Entity::power_t::JUMP) InitializeJump();
		else if (power == Entity::power_t::FLOURISH) InitializeFlourish();
		else if (power == Entity::power_t::BOOST) InitializeBoost();
		else if (power == Entity::power_t::TROOP) AddTroop();
		else if (power == Entity::power_t::SHOOT) Shoot();
	}
private:
	void InitializeJump(void)
	{
		if (!m_jd.jumping)
		{
			m_jd.jumping = true;
			m_jd.gravity = glm::vec3(0.0f, -9.8f, 0.0f);
			m_jd.velocity = glm::vec3(0.0f, 3.5f, 0.0f);
		}
	}
	void InitializeFlourish(void)
	{
		if (!m_fd.flourishing && !m_fd.disabled)
		{
			m_fd.flourishing = true;
			m_fd.gravity = glm::vec3(0.0f, -9.8f, 0.0f);
			m_fd.velocity = glm::vec3(0.0f, 3.5f * 15.0f, 0.0f);
			m_fd.angle = 0.0f;
			m_fd.colorIndex = 0;
		}
	}
	void InitializeBoost(void)
	{
		if (!m_bd.boosting && !m_fd.flourishing && m_bd.allowedToBoost)
		{
			m_bd.boosting = true;
			glm::vec2 plainViewDirection = glm::vec2(m_pData.viewDirection.x, m_pData.viewDirection.z);
			m_bd.viewDirection = plainViewDirection;
			m_bd.resistance = -plainViewDirection * 40.0f;
			m_bd.velocity = plainViewDirection * 60.0f;
		}
	}
	void AddTroop(void)
	{
		if(!m_troopDisabled)
			m_troop.push_back(new Troop(1.0f, m_pData.position));
	}
	void Jump(float deltaT)
	{
		m_pData.position = m_pData.position + m_jd.velocity * deltaT;
		m_jd.velocity = m_jd.velocity + m_jd.gravity * deltaT;
	}
	void Flourish(float deltaT)
	{
		m_pData.position = m_pData.position + m_fd.velocity * deltaT;
		m_fd.velocity = m_fd.velocity + m_fd.gravity * deltaT;

		m_fd.yInclination = static_cast<float>(-(rand() % 9));
		glm::vec3 directionOfBullet = glm::vec3(sin(glm::radians(m_fd.angle)), m_fd.yInclination, cos(glm::radians(m_fd.angle)));
		m_gun.Shoot(directionOfBullet, m_pData.position + glm::vec3(0.0f, 1.0f, 0.0f), m_fd.rainbowColors[m_fd.colorIndex % 7]);
		++m_fd.colorIndex;

		m_fd.yInclination = static_cast<float>(-(rand() % 9));
		directionOfBullet = glm::vec3(sin(glm::radians(180 + m_fd.angle)), m_fd.yInclination, cos(glm::radians(180 + m_fd.angle)));
		m_gun.Shoot(directionOfBullet, m_pData.position + glm::vec3(0.0f, 1.0f, 0.0f), m_fd.rainbowColors[m_fd.colorIndex % 7]);
		++m_fd.colorIndex;

		m_fd.angle += 1.0f;
	}
	void Boost(float deltaT)
	{
		glm::vec3 worldVelocity = glm::vec3(m_bd.velocity.x, m_pData.position.y, m_bd.velocity.y);
		m_pData.position = m_pData.position + worldVelocity * deltaT;
		m_bd.velocity = m_bd.velocity + m_bd.resistance * deltaT;
	}
	void CheckJumpLanding(float terrainHeight)
	{
		if (m_pData.position.y - m_pData.height < terrainHeight)
		{
			m_jd.jumping = false;
			m_pData.position.y = terrainHeight + m_pData.height;
		}
	}
	void CheckFlourishLanding(float terrainHeight)
	{
		if (m_pData.position.y - m_pData.height < terrainHeight)
		{
			m_fd.flourishing = false;
			m_pData.position.y = terrainHeight + m_pData.height;
		}
	}
	void CheckBoostEnd(void)
	{
		if (fabs(m_bd.velocity.x) < 1.0f || fabs(m_bd.velocity.y) < 1.0f)
		{
			m_bd.boosting = false;
		}
	}
public:
	void DrawBullets(Entity::UniData& ud, UniformLocations* locations, DrawData& dd, Entity::Robots_t& vec) override
	{
		m_gun.Draw(ud, locations, dd, vec);
	}
	void Shoot(void)
	{
		m_gun.Shoot(m_pData.viewDirection, m_pData.position);
	}
	const bool Alive(void) override
	{
		return m_lives > 0;
	}
	void RemoveLife(void) override
	{
		--m_lives;
		if (m_lives == 0)
			Die();
	}
	void ViewBob(void)
	{
		ViewBobbing();
	}
public:
	// other getters
	bool* Running(void) override 
	{
		return &m_running; 
	}
	void SpeedUp(void) override 
	{
		m_pData.speed *= m_runningDelta;
	}
	void NeutralizeSpeed(const float& speed) override 
	{
		m_pData.speed = speed;
	}
public:
	void DisablePower(const power_t&& power) override
	{
		if (power == Entity::power_t::FLOURISH) DisableFlourish();
		else if (power == Entity::power_t::BOOST) DisableBoost();
		else if (power == Entity::power_t::TROOP) DisableTroop();
		else return;
	}
	void EnablePower(const power_t&& power) override
	{
		if (power == Entity::power_t::FLOURISH) EnableFlourish();
		else if (power == Entity::power_t::BOOST) EnableBoost();
		else if (power == Entity::power_t::TROOP) EnableTroop();
		else return;
	}
	const bool BulletAiring(void) override
	{
		return m_gun.BulletAiring();
	}	
	void DrawTroops(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, Entity::Robots_t& vec) override
	{
		for (auto& i : m_troop)
		{
			i->Draw(ud, locations, dd, vec);
			if(i->BulletAiring())
				i->DrawBullets(ud, locations, dd, vec);
		}
	}
private:
	// troops stuff
	void CheckDeadTroops(void)
	{
		for (unsigned int i = 0; i < m_troop.size(); ++i)
		{
			if (m_troop[i]->Alive())
			{
				m_troop[i]->DeleteBuffers();
				m_troop.erase(m_troop.begin() + i);
			}
		}
	}
private:
	void DisableFlourish(void) { m_fd.disabled = true; }
	void EnableFlourish(void) { m_fd.disabled = false; }
	void DisableBoost(void) { m_bd.allowedToBoost = false; }
	void EnableBoost(void) { m_bd.allowedToBoost = true; }
	void DisableTroop(void) { m_troopDisabled = true; }
	void EnableTroop(void) { m_troopDisabled = false; }
public:
	// what happens when the player dies
	void Die(void)
	{
		// die code
		std::cout << "player died" << std::endl;
		exit(1);
	}
private:
	glm::vec3 m_up;
	glm::vec2 m_oldMousePosition;
	float m_viewBobbing;
	float m_viewBobbingDelta;
	float m_runningDelta;

	bool m_running;

	Gun m_gun;
	JumpData m_jd;
	BoostData m_bd;
	FlourishData m_fd;
	FPSPlayerData m_pData;
	std::vector<Entity*> m_troop;
	bool m_troopDisabled;

	unsigned int m_lives;
};

#endif