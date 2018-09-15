#pragma once

#include "types.h"
#include "entity.h"

class entity_component_system;

struct icomponent
{
	i32 entity_index;

	virtual auto update(f32 id, std::vector<entity> & entities, entity_component_system const & ecs) -> void {}
	virtual auto destroy(void) -> void { entity_index = -1; }
	auto active(void) -> bool { return entity_index != -1; }
};

/*
 * to create a new type of component :
 *
 * specialize the template for component<T> with a new type
 * e.g. template <> struct component <struct "new type"> : icomponent
 *
 * when deriving from icomponent, the new type of component will inherit
 * the index of the entity that the component is pointing to
 * 
 * to construct icomponent : just need the index of the entity in the array / vector
 *
 * the update function which has to be overriden (comes from icomponent) takes
 * difference in time, vector of entities by ref, and the ecs by const ref
 * this is so that you can access other entities or components from the update method
 * 
 */
template <typename T> struct component : icomponent
{
	/* in icomponent is the index of entity that the component is pointing to */

	T value;
};