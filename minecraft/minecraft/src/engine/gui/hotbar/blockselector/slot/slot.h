#ifndef SLOT_HEADER
#define SLOT_HEADER

#include "../../../../block/block.h"
#include "../../../gui.h"

namespace minecraft
{
	namespace gui
	{
		struct Slot
		{
			Block::block_t type;
			GUI* slotgui;
		};
	}
}

#endif