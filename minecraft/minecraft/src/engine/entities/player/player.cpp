#include "player.h"

#include <glm/gtx/transform.hpp>

namespace minecraft
{
	namespace ent
	{
		Player::Player(void)
			: m_playerViewDirection(1.0f, 0.0f, 0.0f), 
			m_playerPosition(0.23f, 80.0f, 0.23f), UP(0.0f, 1.0f, 0.0f), m_speed(3.0f),
			m_playerData({2.0f}), m_jumping(false)
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
			if (m_playerPosition.y < m_playerData.height + blockUnderneath + 0.1f)
			{
				m_playerPosition.y = blockUnderneath + m_playerData.height;
				m_jumpData.upvelocity = glm::vec3(0.0f, 4.0f, 0.0f);
				m_jumping = false;
			}
			else if (!m_jumping)
			{
				m_playerPosition = m_playerPosition + m_jumpData.upvelocity * deltaT;
				m_jumpData.upvelocity = m_jumpData.upvelocity + *gravity * deltaT;
			}
			else m_jumpData.Update(gravity, deltaT, m_playerPosition);
		}
		void Player::Move(const move_t&& movement, data::Time* time)
		{
			glm::vec3 moveVector = glm::normalize(glm::vec3(m_playerViewDirection.x, 0.0f, m_playerViewDirection.z));
			switch (movement)
			{
			case Entity::move_t::FORWARD:
				m_playerPosition += moveVector * Speed(time);
				break;
			case Entity::move_t::BACKWARD:
				m_playerPosition -= moveVector * Speed(time);
				break;
			case Entity::move_t::JUMP:
				Jump();
				break;
			}
		}
		void Player::Strafe(const strafe_t&& strafe, data::Time* time)
		{
			glm::vec3 moveVector = glm::normalize(glm::cross(m_playerViewDirection, UP));
			switch (strafe)
			{
			case Entity::strafe_t::RIGHT:
				m_playerPosition += moveVector * Speed(time);
				break;
			case Entity::strafe_t::LEFT:
				m_playerPosition -= moveVector * Speed(time);
				break;
			}
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
			}
		}
		float Player::Speed(data::Time* time) const
		{
			return m_speed * static_cast<float>(time->deltaT) * 3.0f;
		}
		void Player::Jump(void)
		{
			if (!m_jumping) m_jumpData.Start();
		}
	}
}