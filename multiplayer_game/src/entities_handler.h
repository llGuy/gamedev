#ifndef _ENTITIES_HANDLER_H_
#define _ENTITIES_HANDLER_H_

#include <glm/glm.hpp>
#include <iterator>
#include <array>
#include <vector>
#include "mtime.h"
#if (defined cpp_no_optional)
#include "optional.hpp"
#else
#include <optional>
#endif
#include <tuple>
#include "event.h"
#include "entity.h"
#include "camera.h"
#include "terrain.h"
#include "entity_model.h"

namespace mulgame {

    class EntitiesHandler
    {
    public:
	EntitiesHandler(void);

	Entity& PushEntity(const glm::vec3& position, const glm::vec3& direction, bool isLocal = false);
	void BindCamera(uint32_t index);

	std::optional<Entity*> EViaUsername(const std::string& username);

	void Update(Terrain& terrain);

	// for entity movement
	void Handle(movement_t);
	// for cursor movement (looking around the world)
	void Handle(const glm::vec2& cursorDiff);
	// handle shooting, etc... for any entity
	// bound entity is always going to be zero
	void Handle(ability_t, int32_t index = 0);
	inline
	float Timedelta(void)
	    {
		return static_cast<float>(m_timer.TimeDelta());
	    }
	inline
	void StartTimer(void)
	    {
		m_timer.StartTimer();
	    }
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
	EntityModel& EModel(void)
	    {
		return m_entityModel;
	    }
	inline
	EntityModel& BModel(void)
	    {
		return m_bulletModel;
	    }
	inline
	std::vector<Entity>::iterator ebegin(void)
	    {
		return m_entities.begin();
	    }
	inline
	std::vector<Entity>::iterator eend(void)
	    {
		return m_entities.end();
	    }

	inline
	std::vector<Bullet>::iterator bbegin(void)
	    {
		return m_airingBullets.begin();
	    }
	inline
	std::vector<Bullet>::iterator bend(void)
	    {
		return m_airingBullets.end();
	    }

	uint32_t Size(void) { return m_entities.size(); }
	bool BoundEntityShot(void) { return m_boundEntityShot; }
    private:
	void UpdateBullets(Terrain& terrain);
	void UpdateEntities(Terrain& terrain);

	// query bullet collision may return an entity collision or may not
	using BulletCollisionRV = std::tuple<bool, std::optional<uint32_t>>;
	BulletCollisionRV QueryBulletCollision(Terrain& terrain, Bullet& bullet);
    private:
	// maximum 5 entities
	std::vector<Entity> m_entities;
	std::vector<Bullet> m_airingBullets;
	// user takes control of whichever entity is bound by the camera
	Camera m_camera;
	EntityModel m_entityModel;
	EntityModel m_bulletModel;
	Time m_timer;

	// is camera's bound entity shooting
	bool m_boundEntityShot;
    };

}
#endif /* _ENTITIES_HANDLER_H_ */
