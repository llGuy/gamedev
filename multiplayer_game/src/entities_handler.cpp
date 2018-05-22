#include <string>

#include "entities_handler.h"

namespace mulgame {

    Entity& EntitiesHandler::PushEntity(const glm::vec3& position, const glm::vec3& direction, const std::string& name)
    {
	m_entities.push_back(Entity(position, direction, name));
	return m_entities[m_entities.size() - 1];
    }

    void EntitiesHandler::Update(void)
    {
	m_timer.Update();
    }
    
    void EntitiesHandler::BindCamera(Entity* entity)
    {
	m_camera.Bind(entity);
    }

    void EntitiesHandler::Handle(movement_t movement)
    {
	Entity* controlledEntity = m_camera.BoundEntity();
	std::cout << "entity name " << std::flush << controlledEntity->Name() << std::endl;
	
	controlledEntity->Move(movement, m_timer.TimeDelta());
    }

    void EntitiesHandler::Handle(const glm::vec2& cursorDiff)
    {
        m_camera.Look(cursorDiff, 0.02f);
    }

}
