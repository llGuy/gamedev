#ifndef ENTITY_MOVEMENT_HEADER
#define ENTITY_MOVEMENT_HEADER

#include <stdint.h>

namespace dawn { namespace ent {

		enum movement_t
			: uint32_t
		{
			FORWARD,

			BACKWARD,

			LEFT,

			RIGHT,

			UP,

			DOWN,

			INV
		};

	} 
}

#endif