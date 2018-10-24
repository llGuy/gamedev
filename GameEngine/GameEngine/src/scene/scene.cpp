#include "scene.h"

auto scene::init(void) -> void
{
	/* creates all the systems */
	components.add_system<component_log>(20);
	components.add_system<component_behavior_key>(20);
}

auto scene::init_game_object(game_object_data const & data) -> game_object &
{
	i32 at = objects.add(game_object(data));

	components.ready_object(objects[at]);

	objects[at].object_id() = at;

	return objects[at];
}

auto scene::operator[](u32 index) -> game_object &
{
	return objects[index];
}

auto scene::update(f32 td) -> void
{
	components.update(td, objects);
}