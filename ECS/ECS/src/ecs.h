#pragma once

#include <cassert>
#include <array>
#include <iostream>
#include "common.h"

template <typename T> struct component_type { static i32 value; };
template <typename T> i32 component_type<T>::value = -1;

class ecs
{
private:
	std::vector<isystem *> systems;
public:
	template <typename ... T> auto update_except(f32 td, std::vector<entity> & entities) -> void
	{

	}
	template <typename ... T> auto update_only(f32 td, std::vector<entity> & entities) -> void
	{
		i8 impl[] { ( systems[component_type<T>::value]->update(td, entities), static_cast<i8>(0) )... };
	}
	auto update(f32 td, std::vector<entity> & entities) -> void
	{
		std::for_each(systems.begin(), systems.end(), 
			[&td, &entities](isystem * sys) { sys->update(td, entities); });
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
		handle new_comp_handle = handle{ component_type<T>::value, comp_at };
		subject.components.push_back(new_comp_handle);
	}
public:
	/* removing */
	auto remove(entity & subject) -> void
	{
		for (auto & comp_handle : subject.components)
		{
			auto * sys = get_system(comp_handle.type);
			sys->remove(comp_handle.index);
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
};