#include "player.h"

namespace dawn { namespace ent {

		Player::Player(const glm::vec3& pos, const glm::vec3& dir)
			: Entity(pos, dir)
		{
		}

		void Player::Move(movement_t type, float time)
		{
			glm::vec3 move = glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z));
			switch (type)
			{
			case movement_t::FORWARD: { /* do nothing */break; }
			case movement_t::BACKWARD: { /* inverse the direction */move *= -1.0f; break; }
			default: { move = glm::vec3(0.0f); break; }
			}
			m_position += move / time;
		}

		void Player::VMove(vmovement_t type, float time)
		{
			glm::vec3 vmove = glm::vec3(0.0f, 1.0f, 0.0f);
			switch (type)
			{
			case vmovement_t::UP: { /* do nothing */break; }
			case vmovement_t::DOWN: { /* inverse the direction */vmove *= -1.0f; break; }
			default: { vmove = glm::vec3(0.0f); break; }
			}
			m_position += vmove / time;
		}
		void Player::Strafe(strafe_t type, float time)
		{
			glm::vec3 strafe = glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));
			switch (type)
			{
			case strafe_t::RIGHT: { /* do nothing */break; }
			case strafe_t::LEFT: { strafe *= -1.0f; break; }
			default: { strafe = glm::vec3(0.0f); break; }
			}
			m_position += strafe / time;
		}

	} 
}