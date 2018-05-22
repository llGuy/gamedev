#ifndef _ENTITIES_HANDLER_H_
#define _ENTITIES_HANDLER_H_

#include <vector>
#include "time.h"
#include "event.h"
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

	void Update(void);

	// for entity movement
	void Handle(movement_t);
	// for cursor movement (looking around the world)
	void Handle(const glm::vec2& cursorDiff);
	
	inline
	Entity& operator[](uint32_t i)
	{
	    return m_entities[i];
	}
	inline
	Camera& Cam(void)
	{
	    return m_camera;
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

	Time m_timer;
    };

}

#endif /* _ENTITIES_HANDLER_H_ */
