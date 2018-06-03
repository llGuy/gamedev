#include <iostream>

#include "entity.h"

namespace mulgame {

    Entity::Entity(bool isLocal)
	: m_terraforming(-1), m_address(isLocal), m_username("INV")
    {
    }
    
    Entity::Entity(const glm::vec3& position, const glm::vec3& direction, uint32_t id, bool isLocal)
	: m_position(position), m_direction(direction), m_entityID(id), m_height(3.0f), m_terraforming(-1), m_speed(DEFAULT_SPEED), m_address(isLocal), m_username("INV")
    {
    }

    // needs the terrain to know whether or not the player is at ground level
    void Entity::Move(movement_t mtype, float timeDelta)
    {
	static constexpr float GRAVITY = -1.0f;
	static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
		
	glm::vec3 xzDirection = glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z));
	glm::vec3 moveDirection;

	switch(mtype)
	{
	case movement_t::RUN:	   m_speed = DEFAULT_SPEED * 2.0f; return;
	case movement_t::FORWARD:  moveDirection = xzDirection; break;
	case movement_t::BACKWARD: moveDirection = -xzDirection; break;
	case movement_t::RIGHT:    moveDirection = glm::cross(xzDirection, UP); break;
	case movement_t::LEFT:     moveDirection = -glm::cross(xzDirection, UP); break;
	case movement_t::JUMP:	   
	    bool atGroundHeight = (m_groundLevel > m_position.y || Equf(m_position.y, m_groundLevel));
	    m_dataJump.Jump(timeDelta, GRAVITY, atGroundHeight); 
	    m_position.y += m_dataJump.upforce; 
	    return;
	}
	m_position += moveDirection * timeDelta * m_speed;
    }

    void Entity::UpdateData(float groundHeight, float timedelta)
    {
	static constexpr float GRAVITY = -1.0f;
	// y value of player position 
	// is treated differently from other components
	m_groundLevel = groundHeight;

	m_dataJump.Update(timedelta, GRAVITY, (m_groundLevel > m_position.y || Equf(m_position.y, m_groundLevel)));
	// balance the forces
	//m_position.y += GRAVITY;
	glm::vec3 newp = m_position;
	newp.y += m_dataJump.upforce;
	if (newp.y < m_groundLevel)
	    newp.y = m_groundLevel;
		
	m_position = newp;
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
