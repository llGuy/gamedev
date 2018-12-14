#include "scene.h"
#include "component/component.h"
#include "../animation/animation_component.h"
#include "../animation/animation_render_component.h"
#include "../animation/animation_key_control_component.h"

auto scene::init(input_handler & inputs, model_handler & models, material_handler & materials) -> void
{
	/* creates all the systems */
	components.add_system<component_log>(20);
	components.add_system<component_render>(20);
	components.add_system<component_behavior_key>(20);
	components.add_system<component_model_matrix>(20);
	components.add_system<component_behavior_mouse>(20);
	components.add_system<component_animation3D>(20);
	components.add_system<component_animation3D_render>(20);
	components.add_system<component_animation3D_key_control>(20);

	loader.init(*this, inputs, models, materials);
}

auto scene::load_from_file(void) -> void
{
	loader.load(extract_file("res/saves/entities.json"), *this);
}

auto scene::init_game_object(game_object_data const & data) -> game_object &
{
	i32 at = objects.add(game_object(data));

	components.ready_object(objects[at]);

	/* object id is used for updating the components */
	objects[at].object_id() = at;
	index_map[data.name] = at;

	return objects[at];
}

auto scene::get_game_object(std::string const & name) -> game_object &
{
	return objects[index_map[name]];
}

auto scene::operator[](u32 index) -> game_object &
{
	return objects[index];
}

auto scene::update(f32 td) -> void
{
	components.update(td, objects);
	scene_camera.update_view_matrix(objects);
}

auto scene::get_scene_camera(void) -> camera &
{
	return scene_camera;
}

auto scene::bind_camera_to_object(game_object & subject) -> void
{
	scene_camera.init(subject.id);
	scene_camera.get_direction() = subject->direction;
}