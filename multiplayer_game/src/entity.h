#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <glm/glm.hpp>

namespace mulgame {

    enum movement_t
    {
	FORWARD,

	BACKWARD,

	LEFT,

	RIGHT,

	JUMP
    };
    
    class Entity
    {
    public:
	Entity(void) = default;
	Entity(const glm::vec3& position, const glm::vec3& direction);

	void Move(movement_t mtype);
	void ModifyDirection(const glm::vec3& direction);
    public:
	inline
	const glm::vec3& Position(void) const
	{
	    return m_position;
	}
	inline
	glm::vec3& Direction(void) 
	{
	    return m_direction;
	}
    private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
    };

}

#endif /* _ENTITY_H_ */
