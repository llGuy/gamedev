#include "render_update_mutex.h"

namespace minecraft
{
	namespace chunk
	{
		namespace cmap
		{
			std::mutex rumutex;
			std::mutex dumutex;
		}
	}
}