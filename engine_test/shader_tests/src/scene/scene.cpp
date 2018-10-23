#include "scene.h"
#include "component_log.h"

auto scene::init_systems(void) -> void
{
	components.add_system<component_log>(1);
}

auto scene::submit_game_object(game_object const & object) -> void
{
	i32 at = objects.add(std::move(object));

	objects[at].object_id() = at;

	components.ready_object(objects[at]);
}

auto scene::operator[](u32 index) -> game_object &
{
	game_object & object = objects[0];

	return objects[index];
}

auto scene::update_render(f32 td) -> void
{

}

auto scene::update(f32 td) -> void
{
	components.update(td, objects);
}