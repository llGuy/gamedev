#include "joint.h"

joint::joint(std::string const & name, u32 id)
	: id(id), name(name)
{
}

auto joint::add_child(joint * child) -> void
{
	children.push_back(child);
}

auto joint::operator[](u32 index) -> joint * &
{
	return children[index];
}

auto joint::get_id(void) -> u32 &
{
	return id;
}

auto joint::get_name(void) -> std::string &
{
	return name;
}

auto joint::get_local_bind_transform(void) -> glm::mat4 &
{
	return local_bind_transform;
}

auto joint::get_parent(void) ->  joint * &
{
	return parent;
}

auto joint::get_inverse_bind_transform(void) -> glm::mat4 &
{
	return inverse_bind_transform;
}

auto joint::get_animated_transform(void) -> glm::mat4 &
{
	return animated_transform;
}

auto joint::get_child_count(void) const -> u32
{
	return children.size();
}