#include "key_control.h"
#include "mouse_control.h"
#include "entity_handler.h"
#include "component/render.h"
#include "component/model_matrix.h"

auto entity_handler::create(input_handler & ih) -> void
{
	component_system.add_system<mouse_control>(4);
	component_system.add_system<basic_key_control>(4);
	component_system.add_system<render_color>(20);
	component_system.add_system<model_matrix>(20);
	create_main_player(ih);

	pre_render_cam_pos.camera_position = glm::vec3(0.0f);
}

auto entity_handler::update(f32 td) -> void
{
	component_system.update_except<render_color>(td, entities);
	cam.update_view_matrix(entities);

	pre_render_cam_pos.camera_position = cam.pos();
}

auto entity_handler::get_camera(void) -> camera &
{
	return cam;
}

auto entity_handler::get_pre_render_cam_pos(void) -> pre_render_camera_position &
{
	return pre_render_cam_pos;
}

auto entity_handler::create_main_player(input_handler & ih) -> void
{
	entity main;
	auto & data = main.get_data();

	data.speed = 10;
	data.pos = glm::vec3(0.0f);
	data.vel = glm::vec3(0.0f);
	data.dir = glm::vec3(1.0f, 0.0f, 0.0f);

	i32 at = entities.add(main);
	bound_entity = at;
	cam.bind_entity(bound_entity, entities);
	component_system.add_component<basic_key_control>(main, at, ih);
	component_system.add_component<mouse_control>(main, at, ih, cam);
}

auto entity_handler::submit_entity(entity_init_data const & init_data) -> void
{
	entity next;

	auto & data = next.get_data();

	data.speed = 0.0f;
	data.pos = init_data.position;
	data.dir = init_data.direction;
	data.vel = glm::vec3(0.0f);
	data.size = init_data.scale;

	i32 at = entities.add(next);
	component_system.add_component<model_matrix>(next, at);
	component_system.add_component<render_color>(next, at, init_data.renderer);
}