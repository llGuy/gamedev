#include <iostream>

#include "entity.h"

namespace mulgame {

    Entity::Entity(void)
	: m_name("untitled")
    {
    }
    
    Entity::Entity(const glm::vec3& position, const glm::vec3& direction, const std::string& name)
	: m_position(position), m_direction(direction), m_name(name)
    {
    }

    void Entity::Move(movement_t mtype, float timeDelta)
    {
	static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	
	glm::vec3 moveDirection;

	switch(mtype)
	{
	case movement_t::FORWARD:  moveDirection = m_direction; break;
	case movement_t::BACKWARD: moveDirection = -m_direction; break;
	case movement_t::RIGHT:    moveDirection = glm::cross(m_direction, UP); break;
	case movement_t::LEFT:     moveDirection = -glm::cross(m_direction, UP); break;
	}
	m_position += moveDirection * 0.02f;
    }

    void Entity::ModifyDirection(const glm::vec3& direction)
    {
	// switch the direction
	m_direction = direction;
    }
    
}
