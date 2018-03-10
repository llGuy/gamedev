#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <glm/glm.hpp>

#include "../data/time_data.h"

namespace minecraft
{
	namespace ent
	{
		class Entity
		{
		public:
			enum class move_t
			{
				FORWARD,

				BACKWARD,

				JUMP,

				CROUCH,

				TOPLAYER		// for zombies if they get implemented
			};

			enum class strafe_t
			{
				RIGHT,

				LEFT
			};

			/* vertical movement */
			enum class vmove_t
			{
				UP, 

				DOWN,

				TOGGLE_FLY
			};

			enum class state_t
			{
				TOGGLE_FLY
			};

			virtual ~Entity(void)
			{
			}
			/* so that the camera can bind to a certain entity */
			virtual glm::vec3* EntityViewDirection(void) { return nullptr; }
			virtual glm::vec3* EntityWorldPosition(void) { return nullptr; }

			virtual void UpdData(glm::vec3* gravity, float blockUnderneath, float deltaT) {}
			virtual void UpdData(glm::vec3* gravity, bool blockUnderneathPresent, float deltaT) {}
			virtual void Move(const move_t&& movement, data::Time* time, bool obstx[2], bool obstz[2]) {}
			virtual void Strafe(const strafe_t&& strafe, data::Time* time, bool obstx[2], bool obstz[2]) {}
			virtual void VMove(const vmove_t&& vmovement, data::Time* time) {}
			virtual void ToggleState(const state_t&& state) {}
			virtual void SpeedUp(void) {}
			virtual bool AttainedMaxJHeight(void) { return false; }
			virtual bool Jumping(void) { return false; }
			virtual float Height(void) { return 0; };
		};
	}
}

#endif