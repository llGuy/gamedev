#ifndef MEMORY_MANAGER_HEADER
#define MEMORY_MANAGER_HEADER
#include <vector>
#include <iostream>

namespace dawn { namespace utils  {

		struct Allocation
		{
			Allocation(void)
				: ptr(nullptr), allocated(false)
			{
			}
			Allocation(void* p)
				: ptr(p), allocated(true)
			{
			}
			bool allocated;
			void* ptr;
		};

		class MemoryManager
		{
		public:
			static MemoryManager& Shared(void);

			MemoryManager(const MemoryManager&) = delete;
			const MemoryManager& operator=(const MemoryManager&) = delete;
			MemoryManager(void) = default;
			~MemoryManager(void) = default;

			template<typename _Ty, typename... _Args>
			__forceinline
			_Ty* Alloc(_Args&&... constrArgs /*arguments of the constructor*/)
			{
				_Ty* nT = new _Ty(std::forward<_Args>(constrArgs)...);
				if constexpr (DEBUG) m_allocations.emplace_back(nT);
				return nT;
			}

			template<typename _Ty>
			__forceinline
			_Ty* AllocArr(const uint32_t& sz)
			{
				_Ty* nT = new _Ty[sz];
				if constexpr (DEBUG) m_allocations.emplace_back(nT);
				return nT;
			}

			template<typename _Ty>
			__forceinline
			void Dealloc(_Ty* ptr)
			{
				if constexpr (DEBUG)
				{
					if (UpdateDealloc(ptr)) delete ptr;
					// error
				}
				else delete ptr;
			}
			
			template<typename _Ty>
			__forceinline
			void DeallocArr(_Ty* ptr)
			{
				if constexpr (DEBUG)
				{
					if (UpdateDealloc(ptr)) delete ptr;
					// error
				}
				else delete[] ptr;
			}
		private:
			const bool UpdateDealloc(void* ptr)
			{
				for (auto& al : m_allocations)
				{
					if (al.ptr == ptr)
					{
						if (al.allocated == false) break;
						else
						{
							al.allocated = false;
						}
						return true;
					}
				}
				return false;
			}
		private:
			static constexpr bool DEBUG = false;
			std::vector<Allocation> m_allocations;
		};

	}	
}

#endif