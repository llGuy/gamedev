#include "player.h"

namespace dawn {namespace ent {

		Player::Player(const glm::vec3& pos, const glm::vec3& dir)
			: Entity(pos, dir)
		{
		}

		void Player::Move(const movement_t& type, float time)
		{
			
		}

	} 
}