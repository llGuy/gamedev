#ifndef _BULLET_HEADER_
#define _BULLET_HEADER_

#include <glm/glm.hpp>

namespace mulgame {

	struct Bullet
	{
		Bullet(const glm::vec3& position, const glm::vec3& direction, uint32_t id)
			: pos(position), dir(direction), shooterID(id)
		{
		}

		void Update(float timeDelta)
		{
			pos += dir * timeDelta * BULLET_SPEED;
		}
		
		static constexpr float BULLET_SPEED = 20.0f;
		glm::vec3 pos;
		glm::vec3 dir;
		uint32_t shooterID;
	};

}

#endif
