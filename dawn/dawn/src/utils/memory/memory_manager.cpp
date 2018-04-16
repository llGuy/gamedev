#include "memory_manager.h"

namespace dawn { namespace utils {

		MemoryManager& MemoryManager::Shared(void)
		{
			static MemoryManager* mm = nullptr;
			if (!mm) mm = new MemoryManager();
			return *mm;
		}

	}
}