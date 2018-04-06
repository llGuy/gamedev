#ifndef ENTITY_MOVEMENT_HEADER
#define ENTITY_MOVEMENT_HEADER

#include <stdint.h>

namespace dawn { namespace ent {

		enum class movement_t
		{
			FORWARD,

			BACKWARD,

			INV
		};

		enum class strafe_t
		{
			LEFT,

			RIGHT,

			INV
		};

		// vertical movement
		enum class vmovement_t
		{
			UP,

			DOWN,

			INV
		};

	} 
}

#endif