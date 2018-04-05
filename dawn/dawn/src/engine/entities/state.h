#ifndef ENTITY_STATE_HEADER
#define ENTITY_STATE_HEADER

#include <stdint.h>

namespace dawn { namespace ent {
		
		using State = bool;

		enum entitystate_t
			: uint32_t
		{
			GRAVITY_STATE,

			RUNNING_STATE,

			INV
		};

		enum gravitystate_t
			: bool
		{
			ENABLED = true,

			DISABLED = false
		};

		enum runningstate_t
			: bool
		{
			RUNNING = true,

			WALKING = false
		};

	} 
}

#endif