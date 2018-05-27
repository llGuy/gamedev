#define GLM_ENABLE_EXPERIMENTAL

#include <algorithm>
#include <string>
#include <tuple>

#include "entities_handler.h"

namespace mulgame {

	EntitiesHandler::EntitiesHandler(void)
		: m_entityModel(1.0f), m_bulletModel(0.1f)
	{
	}

	Entity& EntitiesHandler::PushEntity(const glm::vec3& position, const glm::vec3& direction)
	{
		m_entities.push_back(Entity(position, direction, m_entities.size()));
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

	void EntitiesHandler::Handle(ability_t ability)
	{
		Entity& controlledEntity = m_entities[m_camera.BoundEntity()];
		auto abret = controlledEntity.PerformAbility(ability);

		if (abret.has_value())
		{
			// push bullet into data structure
			m_airingBullets.push_back(abret.value());
		}
	}

	void EntitiesHandler::UpdateEntities(Terrain& terrain)
	{
		std::for_each(m_entities.begin(), m_entities.end(),
			[&](Entity& entity) -> void { entity.UpdateData(terrain.EntityHeight(entity.Position()), m_timer.TimeDelta()); });
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

}
