#ifndef MEMORY_MANAGER_HEADER
#define MEMORY_MANAGER_HEADER

#include <vector>

namespace minecraft
{
	namespace dbg
	{
		struct Allocation
		{
			Allocation(void) = default;
			Allocation(void* p)
				: ptr(p), allocated(true)
			{
			}

			bool allocated;
			void* ptr;
		};

		// singleton
		class MemoryManager
		{
		private:
			explicit MemoryManager(void) = default;
		public:
			static MemoryManager& Shared(void);

			template<typename _T>
			_T* Allocate(void)
			{
				Allocation a(new _T);
				m_allocations.push_back(a);
				return static_cast<_T*>(a.ptr);
			}
			template<typename _T>
			void Deallocate(_T* p)
			{
				bool found = false;
				for (auto& i : m_allocations)
					if (i.ptr == p)
					{
						i.allocated = false; 
						found = true;
						break;
					}
				
				if (!found)
					std::cout << "error" << std::endl;
				else delete p;
			}
		private:
			std::vector<Allocation> m_allocations;
		};
	}
}

#endif