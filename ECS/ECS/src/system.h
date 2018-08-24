#pragma once

#include <new>
#include <stack>
#include <vector>
#include <utility>
#include <algorithm>
#include "entity.h"
#include "component.h"

class isystem
{
public:
	/* ADD FUNCTION!!! */
	virtual auto update(f32 td, std::vector<entity> & entities) -> void = 0;
	virtual auto remove(i32 at) -> void = 0;
};

template <typename T> class component_system : public isystem
{
private:
	std::vector<component<T>> components;
	/* when a component is removed, the index of 
	that component gets pushed for possible re-use */
	std::stack<i32, std::vector<i32>> bin;
public:
	using type = T;

	component_system(i32 initial_size)
	{
		components.reserve(initial_size);
	}

	auto update(f32 td, std::vector<entity> & entities) -> void override
	{
		std::for_each(components.begin(), components.end(),
			[&td, &entities](component<T> & comp) { if (comp.active()) comp.update(td, entities); });
	}
public:
	template <typename ... Args> auto add(Args && ... constr_args) -> i32
	{
		i32 index = components.size();
		if (bin.empty()) components.emplace_back(std::forward<Args>(constr_args)...);
		else
		{
			/* construct in-place */
			index = bin.top();
			auto * at_ptr = &(components[ index ]);
			new(at_ptr) component<T>(std::forward<Args>(constr_args)...);
			bin.pop();
		}
		return index;
	}

	auto remove(i32 at) -> void override
	{
		components[at].destroy();
		bin.push(at);
	}
};