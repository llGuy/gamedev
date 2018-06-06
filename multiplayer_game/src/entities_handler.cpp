#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif


#include <algorithm>
#include <string>
#include <tuple>

#include "entities_handler.h"

namespace mulgame {

    EntitiesHandler::EntitiesHandler(void)
	: m_entityModel(1.0f), m_bulletModel(0.1f)
    {
    }

    Entity& EntitiesHandler::PushEntity(const glm::vec3& position, const glm::vec3& direction, bool isLocal)
    {
	m_entities.push_back(Entity(position, direction, m_entities.size(), isLocal));
	return m_entities[m_entities.size() - 1];
    }

    Entity& EntitiesHandler::PushEntity(const std::string& username)
    {
	m_entities.push_back(Entity(username, m_entities.size()));
	return m_entities[m_entities.size() - 1];
    }

    void EntitiesHandler::Update(Terrain& terrain)
    {
	m_timer.Update();
	UpdateEntities(terrain);
	UpdateBullets(terrain);
	m_entities[m_camera.BoundEntity()].ResetSpeed();
    }

    void EntitiesHandler::BindCamera(uint32_t index)
    {
	m_camera.Bind(index);
    }

    void EntitiesHandler::Handle(movement_t movement)
    {
	Entity& controlledEntity = m_entities[m_camera.BoundEntity()];
	controlledEntity.Move(movement, static_cast<float>(m_timer.TimeDelta()));

	m_camera.UpdateViewMatrix(controlledEntity);
    }

    void EntitiesHandler::Handle(const glm::vec2& cursorDiff)
    {
	Entity& controlledEntity = m_entities[m_camera.BoundEntity()];
	m_camera.Look(controlledEntity, cursorDiff, 0.02f);
    }

    void EntitiesHandler::Handle(ability_t ability, int32_t index)
    {
	Entity& entity = m_entities[index];
	auto abret = entity.PerformAbility(ability);

	if (abret.has_value())
	{
	    // push bullet into data structure
	    m_airingBullets.push_back(abret.value());
	    entity.Shot() = true;
	}
    }

    void EntitiesHandler::UpdateEntities(Terrain& terrain)
    {
	// no need to update client players
	m_entities[0].UpdateData(terrain.EntityHeight(m_entities[0].Position()), m_timer.TimeDelta());

	// every frame reset bit
	for(auto& entity : m_entities) entity.Shot() = false;
    }

    void EntitiesHandler::UpdateBullets(Terrain& terrain)
    {
	std::for_each(m_airingBullets.begin(), m_airingBullets.end(),
		      [&](Bullet& bullet) -> void { bullet.Update(m_timer.TimeDelta()); });

	uint32_t eliminatedBullets = 0;
	for (uint32_t i = 0; i < m_airingBullets.size(); ++i)
	{
	    auto bcrv = QueryBulletCollision(terrain, m_airingBullets[i]);
	    if (std::get<bool>(bcrv))
	    {
		m_airingBullets[i] = m_airingBullets[m_airingBullets.size() - eliminatedBullets - 1];
		eliminatedBullets++;
	    }
	}
	for (uint32_t i = 0; i < eliminatedBullets; ++i) m_airingBullets.pop_back();
    }

    EntitiesHandler::BulletCollisionRV EntitiesHandler::QueryBulletCollision(Terrain& terrain, Bullet& bullet)
    {
	// if bullet hit the terrain
	float terrainHeight = terrain.EntityHeight(bullet.pos);
	if (Equf(terrainHeight, bullet.pos.y) || terrainHeight > bullet.pos.y)
	    return std::make_tuple<bool, std::optional<uint32_t>>(true, std::optional<uint32_t>{ /* empty */ });

	uint32_t entityIndex = 0;
	for (; entityIndex < m_entities.size(); ++entityIndex)
	{
	    if (entityIndex != bullet.shooterID)
	    {
		float distance = glm::distance(bullet.pos, m_entities[entityIndex].Eye());
		// temporary
		static constexpr float BULLET_RADIUS = 0.1f;
		if (distance < 0.1f + m_entityModel.Radius())
		    return std::make_tuple<bool, std::optional<uint32_t>>(true, std::optional<uint32_t>(entityIndex));
	    }
	}
	return std::make_tuple<bool, std::optional<uint32_t>>(false, std::optional<uint32_t>());
    }

    std::optional<Entity*> EntitiesHandler::EViaUsername(const std::string& username)
    {
/*	for(auto& entity : m_entities)
	{
	    if(entity.Username() == username)
	    {
		return &entity;
	    }
	}*/
	for(uint32_t i = 0; i < m_entities.size(); ++i)
	{
	    if(m_entities[i].Username() == username)
	    {
		return &m_entities[i];
	    }
	}
	return std::optional<Entity*> {};
    }

}
