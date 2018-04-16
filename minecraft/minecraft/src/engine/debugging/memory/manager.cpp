#include "manager.h"

namespace minecraft
{
	namespace dbg
	{
		MemoryManager& MemoryManager::Shared(void)
		{
			static MemoryManager* mmanager = nullptr;
			if (!mmanager) mmanager = new MemoryManager;
			return *mmanager;
		}
	}
}