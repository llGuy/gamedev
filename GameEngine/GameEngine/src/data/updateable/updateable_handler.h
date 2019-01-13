#pragma once

#include <vector>
#include <unordered_map>
#include "../../utils/detail.h"

struct updateable_base
{
	virtual auto update(void) -> void = 0;
};

template<typename Callable, typename T> struct updateable : updateable_base
{
	using type = T;

	Callable f;
	T value;

	updateable(Callable const & f, T const & value = T{})
		: f(f), value(value)
	{
	}

	auto update(void) -> void override
	{
		f(value);
	}
};

class updateable_handler
{
private:
	struct group
	{
		std::vector<updateable_base *> updateables;
		std::unordered_map<std::string, u32> index_map;
	};

	std::unordered_map<hashed_string_value_type, group, string_hash<hashed_string_value_type>> groups;
public:
	template <typename Callable, typename T> auto init_updateable(hashed_string const & group_name, std::string const & name, Callable && f, T const & value = T{}) -> updateable<Callable, T> *
	{
		auto & grp = groups[hashed_string_value_type{ group_name.hashed_value }];

		grp.index_map[name] = grp.updateables.size();
		updateable<Callable, T> * ptr = new updateable(f, value);

		grp.updateables.push_back(ptr);

		return ptr;
	}

	auto update(hashed_string const & str) -> void
	{
		auto & grp = groups[hashed_string_value_type{ str.hashed_value }];

		for (u32 i = 0; i < grp.updateables.size(); ++i)
		{
			grp.updateables[i]->update();
		}
	}
};