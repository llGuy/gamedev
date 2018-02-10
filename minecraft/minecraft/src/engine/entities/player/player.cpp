#include "player.h"

#include <glm/gtx/transform.hpp>

namespace minecraft
{
	namespace ent
	{
		Player::Player(void)
			: m_playerViewDirection(1.0f, 0.0f, 0.0f), 
			m_playerPosition(0.0f, 50.0f, 0.0f), UP(0.0f, 1.0f, 0.0f), m_speed(3.0f)
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

		void Player::UpdData(void)
		{
			// when jumping
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
			return m_speed * static_cast<float>(time->deltaT) * 10.0f;
		}
	}
}