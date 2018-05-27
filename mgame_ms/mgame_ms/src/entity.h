#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "math.h"
#include "bullet.h"
#include <string>
#if (defined __cplusplus) && (__cplusplus >= 201700L)
#include <optional>
#else
#include <optional>
//#include "optional.hpp"
#endif
#include <glm/glm.hpp>

namespace mulgame {

	enum movement_t
	{
		FORWARD,

		BACKWARD,

		LEFT,

		RIGHT,

		JUMP,

		RUN
	};

	enum ability_t
	{
		SHOOT
	};

	struct JData
	{
		JData(void)
			: jumping(false)
		{
			Balance();
		}
		void Jump(float timedelta, float gravity, bool atGroundHeight)
		{
			if (atGroundHeight)
			{
				upforce = 0.0f;
				velocity = 0.35f;
				Move(timedelta, gravity);
			}
		}
		void Update(float timedelta, float gravity, bool atGroundHeight)
		{
			// not at ground height, or the player isn't jumping
			if (!atGroundHeight) Move(timedelta, gravity);
			else Balance();
		}
		void Move(float timedelta, float gravity)
		{
			if (!jumping)
			{
				upforce = upforce + velocity * timedelta;
				velocity = velocity + gravity * timedelta;
			}
		}
		void Balance(void)
		{
			upforce = 0.0f;
			velocity = 0.0f;
			jumping = false;
		}
		float upforce;
		float velocity;
		bool jumping;
	};

	class Entity
	{
	public:
		Entity(void);
		Entity(const glm::vec3& position, const glm::vec3& direction, uint32_t id);

		void Move(movement_t mtype, float timeDelta);
		// function may or may not return a bullet
		std::optional<Bullet> PerformAbility(ability_t ability);
		void ModifyDirection(const glm::vec3& direction);
		void UpdateData(float y, float timedelta);
	public:
		const glm::vec3& Position(void) const { return m_position; }
		glm::vec3& Direction(void) { return m_direction; }
		uint32_t ID(void) { return m_entityID; }
		int32_t& Terraforming(void) { return m_terraforming; }
		glm::vec3 Eye(void) 
		{
			glm::vec3 p(m_position);
			p.y += m_height;
			return p;
		}
		void ResetSpeed(void) { m_speed = DEFAULT_SPEED; }
	private:
		static constexpr float DEFAULT_SPEED = 6.0f;

		glm::vec3 m_position;
		glm::vec3 m_direction;
		JData m_dataJump;
		uint32_t m_entityID;

		float m_height;
		float m_speed;
		float m_groundLevel;

		// -1 means that entity is not terraforming
		int32_t m_terraforming;
	};

}

#endif /* _ENTITY_H_ */
