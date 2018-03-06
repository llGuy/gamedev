#include "guieventhandler.h"
#include "../guitype.h"

namespace minecraft
{
	namespace gui
	{
		void GUIEventHandler::HandlerEvent(const event_t& e, 
			GUI** guis, int32_t slot /* for the hotbar */)
		{
			switch (e)
			{
			case event_t::NUMBER:
				guis[BLOCKSELECTOR]->Update(slot);
				break;
			}
		}
	}
}