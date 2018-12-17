#include "scene.h"
#include "component/save.h"
#include "component/component.h"
#include "../animation/animation_component.h"
#include "../animation/animation_render_component.h"
#include "../animation/animation_key_control_component.h"
#include "loader/components_loader.h"
#include "loader/loader_functor.h"

auto scene::init(input_handler & inputs, model_handler & models, material_handler & materials) -> void
{
	/* creates all the systems */
	components.add_system<component_log>(20);
	components.add_system<component_save>(20);
	components.add_system<component_render>(20);
	components.add_system<component_behavior_key>(20);
	components.add_system<component_model_matrix>(20);
	components.add_system<component_behavior_mouse>(20);
	components.add_system<component_animation3D>(20);
	components.add_system<component_animation3D_render>(20);
	components.add_system<component_animation3D_key_control>(20);


	scene_loader_functor::data_type data{ nullptr, this, &scene_camera
		, &inputs, &models, &materials };

	loader.init_data(data);

	using loader_type = json_loader<scene_loader_functor>;

	loader.init_functors(
		loader_type::functor_type<position_functor>{ "position" },
		loader_type::functor_type<direction_functor>{ "direction" },
		loader_type::functor_type<scale_functor>{ "scale" },

		loader_type::functor_type<key_control_functor>{ "key_control"},
		loader_type::functor_type<mouse_control_functor>{ "mouse_control" },
		loader_type::functor_type<model_matrix_functor>{ "model_matrix" },
		loader_type::functor_type<bind_camera_functor>{ "bind_camera" },
		loader_type::functor_type<render_functor>{ "render" },
		loader_type::functor_type<save_functor>{ "save" }
	);
}

auto scene::load_from_file(void) -> void
{
	loader.load(extract_file("res/saves/entities.json")
		, [](scene_loader_functor::data_type & data) {

		game_object_data entity_data;
		entity_data.name = "game_object." + data.iterator->key();

		ENG_INFO("Loaded entity : ", entity_data.name);

		data.dest->init_game_object(entity_data);

		data.obj = &data.dest->get_game_object(entity_data.name);

	});
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

auto scene::destroy(void) -> void
{
	components.destroy_all(objects);
}