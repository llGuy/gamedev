#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "../entity.h"

#include <iostream>

namespace minecraft
{
	namespace ent
	{
		class Player
			: public Entity
		{
		public:
			struct Name
			{
				std::string name;
			};
			explicit Player(void);
		public:
			/* so that the camera can bind to a certain entity */
			glm::vec3* EntityViewDirection(void) override;
			glm::vec3* EntityWorldPosition(void) override;

			void UpdData(void) override;
			void Move(const move_t&& movement, data::Time* time) override;
			void Strafe(const strafe_t&& strafe, data::Time* time) override;
			void VMove(const vmove_t&& vmovement, data::Time* time) override;
			float Speed(data::Time* time) const;
		private:
			Name m_name;
			float m_speed;
			const glm::vec3 UP;
			glm::vec3 m_playerPosition;
			glm::mat4 m_translateMatrix;
			glm::vec3 m_playerViewDirection;
		};
	}
}

#endif