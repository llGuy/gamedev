#include "entity.h"

namespace mulgame {

    Entity::Entity(const glm::vec3& position, const glm::vec3& direction)
	: m_position(position), m_direction(direction)
    {
    }

    void Entity::Move(movement_t mtype)
    {
	// check movement type
    }

    void Entity::ModifyDirection(const glm::vec3& direction)
    {
	// switch the direction
	m_direction = direction;
    }
    
}
