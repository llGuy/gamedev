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
		private:
			struct PData
			{
				float height;
			};
			// data for jumping
			struct JData
			{
				void Start(void)
				{
					upvelocity = glm::vec3(0.0f, 3.5f, 0.0f);
					rising = true;
				}
				void Update(glm::vec3* gravity, float deltaT, glm::vec3& playerPosition)
				{
					playerPosition = playerPosition + upvelocity * deltaT;
					upvelocity = upvelocity + *gravity * deltaT;
				}
				glm::vec3 upvelocity;
				bool rising;
			};
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

			void UpdData(glm::vec3* gravity, float blockUnderneath, float deltaT) override;
			void Move(const move_t&& movement, data::Time* time) override;
			void Strafe(const strafe_t&& strafe, data::Time* time) override;
			void VMove(const vmove_t&& vmovement, data::Time* time) override;
			float Speed(data::Time* time) const;
		private:
			void Jump(void);
		private:
			Name m_name;
			float m_speed;
			const glm::vec3 UP;
			glm::vec3 m_playerPosition;
			glm::mat4 m_translateMatrix;
			glm::vec3 m_playerViewDirection;

			float m_heightOfBlockUnderneath;

			glm::vec3* m_currentGravity;
			JData m_jumpData;
			bool m_jumping;

			PData m_playerData;
		};
	}
}

#endif