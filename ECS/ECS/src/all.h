#pragma once

#include <stack>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <unordered_map>

using i8 = int8_t;
using u8 = int8_t;

using i16 = int16_t;
using u16 = int16_t;

using i32 = int32_t;
using u32 = uint32_t;

using i64 = int64_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

class entity_cs;

struct entity
{
	/* unordered map of types to indices */
	std::unordered_map<i32, i32> components;
};

struct icomponent
{
	i32 entity_index;

	virtual auto update(f32 id, std::vector<entity> & entities, entity_cs const & ecs) -> void {}
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

class isystem
{
public:
	virtual auto update(f32 td, std::vector<entity> & entities, entity_cs const & ecs) -> void = 0;
	virtual auto remove(i32 at) -> void = 0;
};

/* component_system contains a contiguous block of memory for storing
 * components of the same type T
 */
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

	auto update(f32 td, std::vector<entity> & entities, entity_cs const & ecs) -> void override
	{
		std::for_each(components.begin(), components.end(),
			[&td, &entities, &ecs](component<T> & comp) { if (comp.active()) comp.update(td, entities, ecs); });
	}
	auto operator[](i32 comp_at) -> component<T> &
	{
		return components[comp_at];
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
			auto * at_ptr = &(components[index]);
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

template <typename T> struct component_type { static i32 value; };
template <typename T> i32 component_type<T>::value = -1;

class entity_cs
{
private:
	std::vector<isystem *> systems;
public:
	template <typename ... T> auto update_except(f32 td, std::vector<entity> & entities) -> void
	{
		for (u32 i = 0; i < systems.size(); ++i)
			if (((component_type<T>::value != i) && ...))
				systems[i]->update(td, entities, *this);
	}
	template <typename ... T> auto update_only(f32 td, std::vector<entity> & entities) -> void
	{
		(systems[component_type<T>::value]->update(td, entities, *this), ...);
	}
	auto update(f32 td, std::vector<entity> & entities) -> void
	{
		std::for_each(systems.begin(), systems.end(),
			[&td, &entities, this](isystem * sys) { sys->update(td, entities, *this); });
	}
public:
	/* adding */
	template <typename T> auto add_system(i32 size) -> void
	{
		/* initialize new instance of "component_type_impl" */
		using new_type = component_type<T>;

		new_type::value = systems.size();
		systems.emplace_back(new component_system<T>(size));
	}
	template <typename T, typename ... Args> auto add_component(entity & subject, Args && ... constr_args) -> void
	{
		if (component_type<T>::value == -1)
		{
			std::cerr << "error <" << __FUNCTION__ << "()> : didn't add system for requested type";
			throw std::runtime_error("cannot add component to non existent system");
		}

		auto & sys = get_system<T>();
		i32 comp_at = sys.add(std::forward<Args>(constr_args)...);
		subject.components.insert({ component_type<T>::value, comp_at });
	}
public:
	/* removing */
	auto remove(entity & subject) -> void
	{
		for (auto & comp_handle : subject.components)
		{
			auto[type, index] = comp_handle;
			auto * sys = get_system(type);
			sys->remove(index);
		}
	}
public:
	/* access to system needs static_pointer_cast */
	template <typename T> auto get_system(void) -> component_system<T> &
	{
		return *static_cast<component_system<T>*>(systems[component_type<T>::value]);
	}
	auto get_system(i32 at) -> isystem *
	{
		return systems[at];
	}
	template <typename T> auto get_component(i32 comp_at) -> component<T> &
	{
		auto & sys = get_system<T>();
		return sys[comp_at];
	}
};


