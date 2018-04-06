#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "../entity.h"

namespace dawn { namespace ent {
		
		class Player
			: public Entity
		{
		public:
			Player(void) = default;
			Player(const glm::vec3& pos, const glm::vec3& direction);
			void Move(const movement_t&, float time) override;
			void VMove(const vmovement_t&, float time) override;
			void Strafe(const strafe_t&, float time) override;
		private:
			
		};

	} 
}

#endif