#pragma once

#include <new>
#include <stdint.h>

namespace memory {

	class allocator
	{
	protected:
		void * start;
		uint32_t allocation_count;
		uint32_t used;
		uint32_t capacity;
	public:
		allocator(uint32_t capacity, void * start)
			: start(start)
			, capacity(capacity)
			, used(0)
			, allocation_count(0)
		{
		}

		virtual ~allocator(void) = default;

		virtual auto alloc(uint32_t size, uint8_t allignment = 4) -> void * = 0;
		virtual auto dealloc(void * ptr) -> void = 0;

		auto get_start(void) const -> void *
		{
			return start;
		}

		auto get_capacity(void) const -> uint32_t
		{
			return capacity;
		}

		auto get_used_memory(void) const -> uint32_t
		{
			return used;
		}

		auto get_alloc_count(void) const -> uint32_t
		{
			return allocation_count;
		}
	};

	template <typename T> auto allocate(allocator & alloc) -> T *
	{
		return new(alloc.alloc(sizeof T, __alignof(T))) T;
	}

	template <typename T> auto allocate(allocator & alloc, T const & t) -> T *
	{
		return new(alloc.alloc(sizeof T, __alignof(T))) T(t);
	}

	template <typename T> auto deallocate(allocator & alloc, T * subject) -> void
	{
		subject->~T();
		alloc.dealloc(subject);
	}

	template <typename T> auto alloc_array(allocator & alloc, uint32_t length) -> T *
	{
		uint32_t header_size = sizeof(uint32_t);

		T * ptr = ( (T *) alloc.alloc(sizeof(T) * length + header_size, __alignof(T) + +header_size));
		*( (uint32_t *)(ptr) - 1 ) = length;

		for (uint32_t i = 0; i < length; ++i)
		{
			new(ptr + i) T;
		}

		return ptr;
	}

	template <typename T> auto dealloc_array(allocator & alloc, T * array) -> T *
	{
		uint32_t length = *((uint32_t *)(array) - 1);

		for (uint32_t i = 0; i < length; ++i)
			array[i].~T();


	}

}