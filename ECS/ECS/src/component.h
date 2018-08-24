#pragma once

#include "types.h"
#include "entity.h"

struct icomponent
{
	i32 entity_index;

	virtual auto update(f32 id, std::vector<entity> & entities) -> void
	{
	}
	virtual auto destroy(void) -> void
	{
		entity_index = -1;
	}
	auto active(void) -> bool
	{
		return entity_index != -1;
	}
};

template <typename T> struct component : icomponent
{
	/* in icomponent is the index of entity that the component is pointing to */

	T value;
};