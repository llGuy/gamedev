#ifndef RENDER_UPDATE_MUTEX
#define RENDER_UPDATE_MUTEX

#include <mutex>

namespace minecraft
{
	namespace chunk
	{
		namespace cmap
		{
			extern std::mutex mutex;
		}
	}
}

#endif