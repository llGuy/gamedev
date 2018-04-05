#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "../entity.h"

namespace dawn { namespace ent {
		
		class Player
			: public Entity
		{
		public:
			Player(const glm::vec3& pos, const glm::vec3& direction);

			void Move(const movement_t&, float time) override;
		private:
			enum gravitystate_t
				: bool
			{
				ENABLED = true,

				DISABLED = false
			} m_gravityState;
		};

	} 
}

#endif