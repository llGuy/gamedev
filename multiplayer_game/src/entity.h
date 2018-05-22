#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
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
	Entity(void);
	Entity(const glm::vec3& position, const glm::vec3& direction, const std::string& name);

	void Move(movement_t mtype, float timeDelta);
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
	inline
	const std::string& Name(void)
	{
	    return m_name;
	}
    private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	std::string m_name;
    };

}

#endif /* _ENTITY_H_ */
