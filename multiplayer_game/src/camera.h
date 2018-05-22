#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include "entity.h"

namespace mulgame {

    class Camera
    {
    public:
	Camera(void);

	void Bind(Entity* entity);
	void NewCPosition(const glm::vec2& pos);
	void Look(const glm::vec2& ncpos, float sensitivity);

	inline
	Entity*& BoundEntity(void)
	{
	    return m_boundEntity;
	}
	inline
	glm::mat4& ViewMatrix(void)
	{
	    return m_viewMatrix;
	}
    private:
	Entity* m_boundEntity;
	glm::vec2 m_cursorPosition;
	glm::mat4 m_viewMatrix;
    };

}

#endif /* _CAMERA_H_ */
