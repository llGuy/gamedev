#pragma once

#include <cassert>
#include <array>
#include <iostream>
#include "common.h"

template <typename T> struct component_type { static i32 value; };
template <typename T> i32 component_type<T>::value = -1;

class entity_component_system
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
			auto [type, index] = comp_handle;
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