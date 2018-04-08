#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <glm/glm.hpp>
#include "entity_movement.h"
#include "state.h"

namespace dawn { namespace ent {

		class Entity
		{
		public:
			Entity(void) = default;
			Entity(const glm::vec3& p, const glm::vec3& d)
				: m_position(p), m_direction(d), m_states{true /* for testing, enable flying */, false}
			{
			}
			virtual ~Entity(void) = default;

			inline
			glm::vec3& Position(void) 
			{
				return m_position;
			}
			inline
			glm::vec3& Direction(void)
			{
				return m_direction;
			}

			inline
			void ToggleState(entitystate_t s)
			{
				m_states[s] ^= true;
			}
		public:
			virtual void Move(movement_t, float time) = 0;
			virtual void VMove(vmovement_t, float time) = 0;
			virtual void Strafe(strafe_t, float time) = 0;
		protected:
			glm::vec3 m_position;
			glm::vec3 m_direction;
			// all types of states : running, flying, etc...
			State m_states[entitystate_t::INV];
		};

	}
}

#endif