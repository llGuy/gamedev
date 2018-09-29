#pragma once

#include <stack>
#include <memory>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <functional>
#include "../types.h"
#include "../vec_dd.h"
#include <unordered_map>

class entity_cs;

struct entity_data
{
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 vel;
	f32 speed;
	glm::vec3 size;
};

/* implementation structure to get index in vector of systems of a certain type of componente */
template <typename T> struct component_type { static i32 value; };
template <typename T> i32 component_type<T>::value = -1;

class entity
{
private:
	/* unordered map of types to indices */
	std::unordered_map<i32, i32> components;
	entity_data data;
public:
	friend entity_cs;

	entity(void) = default;
	auto get_data(void) -> entity_data &
	{
		return data;
	}
	template <typename T> auto get_component_index(void) -> i32
	{
		return components[component_type<T>::value];
	}
	auto add_component(std::pair<i32, i32> const & comp) -> void
	{
		components.insert(comp);
	}
};

class icomponent
{
protected:
	i32 entity_index;
public:
	icomponent(i32 index) : entity_index(index) {}
	virtual ~icomponent(void) = default;
	/* components may need to access other common components such as height or is_flying */
	virtual auto update(f32 id, vec_dd<entity> & entities, entity_cs & ecs) -> void {}
	virtual auto destroy(void) -> void { entity_index = -1; }
	virtual auto active(void) -> bool { return entity_index != -1; }

	auto subject_index(void) -> i32 { return entity_index; }
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
template <typename T> class component : public icomponent
{
public:
	/* in icomponent is the index of entity that the component is pointing to */
	T value;

	component(entity &, i32 index, T val)
		: value(val), icomponent::icomponent(index)
	{
	}
};

class isystem
{
public:
	virtual auto update(f32 td, vec_dd<entity> & entities,
		entity_cs & ecs, std::function<bool(i32)> const &) -> void = 0;
	virtual auto remove(i32 at) -> void = 0;
};

/* component_system contains a contiguous block of memory for storing
 * components of the same type T
 */
template <typename T> class component_system : public isystem
{
private:
	vec_dd<component<T>> components;
public:
	using type = T;

	component_system(i32 initial_size)
		: components(initial_size)
	{
	}

	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs, std::function<bool(i32)> const & func) -> void override
	{
		for (u32 i = 0; i < components.vec_size(); ++i)
			if (components[i].active() && func(components[i].subject_index()))
				components[i].update(td, entities, ecs);
	}
	auto operator[](i32 comp_at) -> component<T> &
	{
		return components[comp_at];
	}
public:
	template <typename ... Args> auto add(Args && ... constr_args) -> i32
	{
		return components.emplace(std::forward<Args>(constr_args)...);
	}

	auto remove(i32 at) -> void override
	{
		components[at].destroy();
		components.remove(at);
	}
};

class entity_cs
{
private:
	std::vector<std::unique_ptr<isystem>> systems;
public:
	template <typename ... T> auto update_except(f32 td, vec_dd<entity> & entities,
		std::function<bool(i32)> const & depr = [](i32) { return true; }) -> void
	{
		for (u32 i = 0; i < systems.size(); ++i)
			if (((component_type<T>::value != i) && ...))
				systems[i]->update(td, entities, *this, depr);
	}
	template <typename ... T> auto update_only(f32 td, vec_dd<entity> & entities,
		std::function<bool(i32)> const & depr = [](i32) { return true; }) -> void
	{
		(systems[component_type<T>::value]->update(td, entities, *this, depr), ...);
	}
	auto update(f32 td, vec_dd<entity> & entities,
		std::function<bool(i32)> const & depr = [](i32) { return true; }) -> void
	{
		std::for_each(systems.begin(), systems.end(),
			[&td, &entities, &depr, this](std::unique_ptr<isystem> & sys) { sys->update(td, entities, *this, depr); });
	}
public:
	/* adding */
	template <typename T> auto add_system(i32 size) -> void
	{
		/* initialize new instance of "component_type_impl" */
		using new_type = component_type<T>;

		new_type::value = systems.size();
		systems.emplace_back(std::make_unique<component_system<T>>(component_system<T>(size)));
	}
	template <typename T, typename ... Args> auto add_component(entity & subject, Args && ... constr_args) -> void
	{
		if (component_type<T>::value == -1)
		{
			std::cerr << "error <" << __FUNCTION__ << "()> : didn't add system for requested type";
			throw std::runtime_error("cannot add component to non existent system");
		}

		auto & sys = get_system<T>();
		i32 comp_at = sys.add(subject, std::forward<Args>(constr_args)...);
		subject.add_component({ component_type<T>::value, comp_at });
	}
public:
	/* removing */
	auto remove(entity & subject) -> void
	{
		for (auto & comp_handle : subject.components)
		{
			auto[type, index] = comp_handle;
			auto & sys = get_system(type);
			sys->remove(index);
		}
	}
public:
	/* access to system needs static_pointer_cast */
	template <typename T> auto get_system(void) -> component_system<T> &
	{
		return *static_cast<component_system<T>*>((systems[component_type<T>::value].get()));
	}
	auto get_system(i32 at) -> std::unique_ptr<isystem> &
	{
		return systems[at];
	}
	template <typename T> auto get_component(i32 comp_at) -> component<T> &
	{
		auto & sys = get_system<T>();
		return sys[comp_at];
	}
};
