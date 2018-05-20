#include "entities_handler.h"

namespace mulgame {

    Entity& EntitiesHandler::PushEntity(const glm::vec3& position, const glm::vec3& direction)
    {
	m_entities.emplace_back(position, direction);
	return m_entities[m_entities.size() - 1];
    }

    void EntitiesHandler::BindCamera(Entity* entity)
    {
	m_camera.Bind(entity);
    }

}
