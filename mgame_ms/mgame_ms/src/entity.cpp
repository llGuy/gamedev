#include <iostream>

#include "entity.h"

namespace mulgame {

    Entity::Entity(void)
    {
    }
    
    Entity::Entity(const glm::vec3& position, const glm::vec3& direction, uint32_t id)
	: m_position(position), m_direction(direction), m_entityID(id), m_height(3.0f)
    {
    }

	// needs the terrain to know whether or not the player is at ground level
    void Entity::Move(movement_t mtype, float timeDelta)
    {
		static constexpr float GRAVITY = -9.5f;
		static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
		
		glm::vec3 xzDirection = glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z));
		glm::vec3 moveDirection;

		switch(mtype)
		{
		case movement_t::FORWARD:  moveDirection = xzDirection; break;
		case movement_t::BACKWARD: moveDirection = -xzDirection; break;
		case movement_t::RIGHT:    moveDirection = glm::cross(xzDirection, UP); break;
		case movement_t::LEFT:     moveDirection = -glm::cross(xzDirection, UP); break;
		case movement_t::JUMP:	   
			
			m_dataJump.Jump(timeDelta, GRAVITY); 
			m_position.y += m_dataJump.upforce; 
			return;
		}
		m_position += moveDirection * timeDelta * 3.0f;
    }

	void Entity::UpdateData(float groundHeight, float timedelta)
	{
		static constexpr float GRAVITY = -9.5f;
		// y value of player position 
		// is treated differently from other components

		if (m_position.y < groundHeight) 
			m_position.y = groundHeight;

		m_dataJump.Update(timedelta, GRAVITY, (groundHeight > m_position.y || Equf(m_position.y, groundHeight)));
		// balance the forces
		//m_position.y += GRAVITY;
		m_position.y += /*-GRAVITY + */m_dataJump.upforce;
	}

    void Entity::ModifyDirection(const glm::vec3& direction)
    {
		// switch the direction
		m_direction = direction;
    }

	std::optional<Bullet> Entity::PerformAbility(ability_t ability)
	{
		switch (ability)
		{
		case ability_t::SHOOT: return Bullet(Eye(), m_direction, m_entityID);
		}
		return std::optional<Bullet>{};
	}
    
}
