#include "player.h"

#include <glm/gtx/transform.hpp>

#define DEBUG

namespace minecraft
{
	namespace ent
	{
		Player::Player(void)
			: m_playerViewDirection(1.0f, 0.0f, 0.0f), 
			m_playerPosition(0.23f, 80.0f, 0.23f), UP(0.0f, 1.0f, 0.0f), m_speed(2.5f),
			m_playerData({1.0f}), m_jumping(false), m_speedCoeff(1.5f), m_flying(false)
		{
		}
		glm::vec3* Player::EntityViewDirection(void)
		{
			return &m_playerViewDirection;
		}
		glm::vec3* Player::EntityWorldPosition(void)
		{
			return &m_playerPosition;
		}

		void Player::UpdData(glm::vec3* gravity, float blockUnderneath, float deltaT)
		{
			if (!m_flying)
			{
				if (m_jumping)
					m_jumpData.Update(gravity, deltaT, m_playerPosition);

				if (fabs(m_playerPosition.y - (m_playerData.height + blockUnderneath)) < 0.1f ||
					m_playerPosition.y < (m_playerData.height + blockUnderneath + 0.01f))
				{
					m_playerPosition.y = blockUnderneath + m_playerData.height;
					m_jumpData.upvelocity = glm::vec3(0.0f, -4.5f, 0.0f);
					m_jumping = false;
				}
				else { if (!m_jumping) Fall(deltaT, *gravity); }
			}
		}
		void Player::UpdData(glm::vec3* gravity, bool blockUnderneathPresent, float deltaT)
		{
			if (!m_flying)
			{
				if (blockUnderneathPresent)
				{
					/* reset */
					m_jumpData.upvelocity = glm::vec3(0.0f, -4.5f, 0.0f);
					m_jumping = false;
				}
				if (m_jumping) m_jumpData.Update(gravity, deltaT, m_playerPosition);
				else if (!blockUnderneathPresent) Fall(deltaT, *gravity);
			}
		}
		void Player::Fall(float deltaT, glm::vec3& gravity)
		{
			m_playerPosition = m_playerPosition + m_jumpData.upvelocity * deltaT;
			m_jumpData.upvelocity = m_jumpData.upvelocity + gravity * deltaT;
		}
		void Player::Move(const move_t&& movement, data::Time* time, bool obstx[2], bool obstz[2])
		{
			glm::vec3 moveVector = glm::normalize(glm::vec3(m_playerViewDirection.x, 0.0f, m_playerViewDirection.z));
			
			switch (movement)
			{
			case Entity::move_t::FORWARD:
				break;
			case Entity::move_t::BACKWARD:
				moveVector *= -1.0f;
				break;
			case Entity::move_t::JUMP:
				if (!m_flying)
				{
					Jump();

					return;
				}
				else
				{
					m_playerPosition += glm::vec3(0.0f, 1.0f, 0.0f) * Speed(time);
					return;
				}
			case Entity::move_t::CROUCH:
				if (m_flying)
				{
					m_playerPosition += glm::vec3(0.0f, -1.0f, 0.0f) * Speed(time);
				}
				return;
			}
			/* positive x obstruction */
			if (obstx[0] && moveVector.x > 0.00001f) moveVector.x = 0.0f;
			/* negative x obstruction */
			else if (obstx[1] && moveVector.x < -0.00001f) moveVector.x = 0.0f;

			if (obstz[0] && moveVector.z > 0.00001f) moveVector.z = 0.0f;
			else if (obstz[1] && moveVector.z < -0.00001f) moveVector.z = 0.0f;

			m_playerPosition += moveVector * Speed(time) * (m_running ? m_speedCoeff : 1.0f);
			/* reset speed */
			m_running = false;
		}
		void Player::Strafe(const strafe_t&& strafe, data::Time* time, bool obstx[2], bool obstz[2])
		{
			glm::vec3 moveVector = glm::normalize(glm::cross(m_playerViewDirection, UP));
			switch (strafe)
			{
			case Entity::strafe_t::RIGHT: break;
			case Entity::strafe_t::LEFT:
				moveVector *= -1.0f;
				break;
			}
			if (obstx[0] && moveVector.x > 0.00001f) moveVector.x = 0.0f;
			/* negative x obstruction */
			else if (obstx[1] && moveVector.x < -0.00001f) moveVector.x = 0.0f;

			if (obstz[0] && moveVector.z > 0.00001f) moveVector.z = 0.0f;
			else if (obstz[1] && moveVector.z < -0.00001f) moveVector.z = 0.0f;

			m_playerPosition += moveVector * Speed(time) * (m_running ? m_speedCoeff : 1.0f);
			/* reset speed */
			m_running = false;
		}
		void Player::VMove(const vmove_t&& vmovement, data::Time* time)
		{
			glm::vec3 moveVector = glm::vec3(0.0f, 1.0f, 0.0f);
			switch (vmovement)
			{
			case Entity::vmove_t::UP:
				m_playerPosition += moveVector * Speed(time);
				break;
			case Entity::vmove_t::DOWN:
				m_playerPosition -= moveVector * Speed(time);
				break;
			case Entity::vmove_t::TOGGLE_FLY:
				m_flying ^= true;
			}
		}
		float Player::Speed(data::Time* time) const
		{
			return m_speed * static_cast<float>(time->deltaT) * m_speed * (m_flying ? 10.0f : 1.0f);
		}
		void Player::Jump(void)
		{
			if (!m_jumping) m_jumpData.Start(m_jumping, glm::round(m_playerPosition.y));
		}
		void Player::SpeedUp(void)
		{
			m_running = true;
		}
		bool Player::AttainedMaxJHeight(void)
		{
			return m_jumpData.arrivedAtMaxHeight;
		}
		bool Player::Jumping(void)
		{
			return m_jumping;
		}
		float Player::Height(void)
		{
			return m_playerData.height;
		}
		void Player::ToggleState(const state_t&& state)
		{
			switch (state)
			{
			case Entity::state_t::TOGGLE_FLY:
				m_flying ^= true;
			}
		}
	}
}