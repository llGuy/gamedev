#ifndef GUIEVENTHANDLER_HEADER
#define GUIEVENTHANDLER_HEADER

#include "../gui.h"

namespace minecraft
{
	namespace gui
	{
		class GUIEventHandler
		{
		public:
			enum event_t
			{
				NUMBER
			};
			explicit GUIEventHandler(void) = default;
			void HandlerEvent(const event_t& e, GUI** guis, int32_t slot = 0);
		};
	}
}

#endif