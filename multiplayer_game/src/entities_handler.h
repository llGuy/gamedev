#ifndef _ENTITIES_HANDLER_H_
#define _ENTITIES_HANDLER_H_

#include <vector>
#include "entity.h"
#include "camera.h"
#include "entity_model.h"

namespace mulgame {

    class EntitiesHandler
    {
    public:
	EntitiesHandler(void) = default;

	Entity& PushEntity(const glm::vec3& position, const glm::vec3& direction);
	void BindCamera(Entity* entity);
	
	inline
	Entity& operator[](uint32_t i)
	{
	    return m_entities[i];
	}
	inline
	Entity* CameraBoundEntity(void)
	{
	    return m_camera.BoundEntity();
	}	
	inline
	EntityModel& Model(void)
	{
	    return m_model;
	}
    public:
	// iterators
	std::vector<Entity>::iterator begin(void)
	{
	    return m_entities.begin();
	}
	std::vector<Entity>::iterator end(void)
	{
	    return m_entities.end();
	}
    private:
	std::vector<Entity> m_entities;
	// user takes control of whichever entity is bound by the camera
	Camera m_camera;
	EntityModel m_model;
    };

}

#endif /* _ENTITIES_HANDLER_H_ */
