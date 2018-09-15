#pragma once

#include <new>
#include <stack>
#include <vector>
#include "types.h"

/* vec_dd = vector with delayed deletion */
template <typename T> class vec_dd
{
private:
	u32 size;
	std::vector<T> list;
	/* pending deletions */
	std::stack<i32, std::vector<i32>> bin_stack;
public:
	vec_dd(void) = default;
	vec_dd(i32 initial_cap)
		: size(0)
	{
		list.reserve(initial_cap);
	}

	auto operator[](i32 index) -> T &
	{
		return list[index];
	}

	/* number of items pushed into the list that aren't destructed / removed */
	auto amount(void) -> u32
	{
		return size;
	}

	/* size of underlying vector */
	auto vec_size(void) -> u32
	{
		return list.size();
	}

	auto add(T const & new_item) -> i32
	{
		i32 index = list.size();
		if (bin_stack.empty()) list.push_back(new_item);
		else
		{
			/* construct in-place */
			index = bin_stack.top();
			auto * at_ptr = &(list[index]);
			new(at_ptr) T(new_item);
			bin_stack.pop();
		}
		++size;

		return index;
	}
	template <typename ... Args> auto emplace(Args && ... constr_args) -> i32
	{
		i32 index = list.size();
		if (bin_stack.empty()) list.emplace_back(std::forward<Args>(constr_args)...);
		else
		{
			/* construct in-place */
			index = bin_stack.top();
			auto * at_ptr = &(list[index]);
			new(at_ptr) T(std::forward<Args>(constr_args)...);
			bin_stack.pop();
		}
		++size;

		return index;
	}
	auto remove(i32 at, bool destroy = false) -> void 
	{
		if (destroy) delete(&list[at]);
		bin_stack.push(at);
		--size;
	}
};