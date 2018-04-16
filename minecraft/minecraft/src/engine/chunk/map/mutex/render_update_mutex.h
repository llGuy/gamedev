#ifndef RENDER_UPDATE_MUTEX
#define RENDER_UPDATE_MUTEX

#include <mutex>

namespace minecraft
{
	namespace chunk
	{
		namespace cmap
		{
			// render / update mutex
			extern std::mutex rumutex;
			// delete / update mutex
			extern std::mutex dumutex;
		}
	}
}

#endif