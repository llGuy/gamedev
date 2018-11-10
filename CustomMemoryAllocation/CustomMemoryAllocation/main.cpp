#include <new>
#include <iostream>

namespace mem {

	class allocator_base
	{
	protected:
		void * start;
		std::size_t capacity;
		std::size_t used;
		std::size_t num_allocations;
	public:
		allocator_base(void * start, std::size_t cap)
			: start(start), capacity(cap), used(0), num_allocations(0)
		{
		}

		virtual ~allocator_base(void)
		{
			delete[] (char *)start;
		}

		virtual auto alloc(std::size_t size, char alignment = 4) -> void * = 0;
		virtual auto dealloc(void * ptr) -> void = 0;

		/* getters */
		auto get_start(void) const -> void *
		{
			return start;
		}
		auto get_cap(void) const -> std::size_t
		{
			return capacity;
		}
		auto get_num_allocations(void) const -> std::size_t
		{
			return num_allocations;
		}
	};

	/* different types of allocators */
	class linear_allocator : public allocator_base
	{
	private:
		void * position;
	public:
		linear_allocator(void * start, std::size_t size)
			: allocator_base::allocator_base(start, size), position(start)
		{
		}

		auto alloc(std::size_t size, char alignment = 4) -> void * override
		{
			void * prev = position;

			char * new_pos = (char *)position + size;
			position = new_pos;

			used += size;
			++num_allocations;

			return prev;
		}

		auto dealloc(void * ptr) -> void override
		{
			/* cannot use this function */
		}

		auto reset(void) -> void
		{
			used = 0;
			num_allocations = 0;
		}
	};

	class stack_allocator : public allocator_base
	{
	private:
		void * position;
	public:
		struct header
		{
			void * prev;
		};

		stack_allocator(void * start, std::size_t size)
			: allocator_base::allocator_base(start, size), position(start)
		{
		}

		auto alloc(std::size_t size, char alignment = 4) -> void * override
		{
			*(header *)position = header{ position };
			char * ret = (char *)position + sizeof(header);
			char * new_pos = (char *)position + size + sizeof(header);
			position = (void *)new_pos;

			position = new_pos;

			used += size;
			++num_allocations;
				
			return ret;
		}

		auto dealloc(void * ptr) -> void override
		{
			header * head = (header *)ptr - 1;

			used -= ((std::size_t)position - (std::size_t)head->prev);
			position = head->prev;
			--num_allocations;
		}
	};

	template <typename T> auto allocate(allocator_base & allocator, T obj = T()) -> T *
	{
		return new(allocator.alloc(sizeof(T), __alignof(T))) T(obj);
	}

	template <typename T> auto deallocate(allocator_base & allocator, T * obj) -> void
	{
		obj->~T();
		allocator.dealloc(obj);
	}

	template <typename T> auto allocator_array(allocator_base & allocator, std::size_t size) -> T *
	{
		/* header is stored in int */
		std::size_t header_size = sizeof(std::size_t);

		T * ptr = (T *)((char *)allocator.alloc(header_size + sizeof(T) * size, __alignof(T)) + header_size);

		*((std::size_t *)(ptr) - 1) = size * sizeof(T);

		for (std::size_t i = 0; i < size; ++i)
		{
			new(&ptr + i) T;
		}

		return ptr;
	}

	template <typename T> auto deallocate_array(allocator_base & allocator, T * array) -> void
	{
		std::size_t length = *((std::size_t *)array - 1);

		for (std::size_t i = 0; i < length; ++i)
		{
			(array + i)->~T();
		}

		allocator.dealloc(array - sizeof(std::size_t));
	}

}

struct some_struct
{
	int x = 42;
	~some_struct(void)
	{
		x = 0;
	}
};

auto main(void) -> int
{
	mem::stack_allocator stack(new char[10000], 10000);

	some_struct * foo = mem::allocate<some_struct>(stack);

	char * str = mem::allocator_array<char>(stack, 12);

	str[0] = 'a';
	str[1] = 'b';
	str[11] = '\0';

	std::cout << str << std::endl;

	mem::deallocate_array<char>(stack, str);
	mem::deallocate<some_struct>(stack, foo);

	std::cin.get();
}