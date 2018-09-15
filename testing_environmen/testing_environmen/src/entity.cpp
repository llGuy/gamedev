#include "entity.h"
#include "ecs/basic_components.h"
#include "ecs/basic_key_control.h"
#include "ecs/mouse_control.h"

entity_handler::entity_handler(void)
{
}

auto entity_handler::create(input_handler & ih) -> void
{
	create_component_system();
	create_main_player(ih);
}

auto entity_handler::update(f32 td) -> void
{
	component_system.update(td, entities);
	entity_camera.update_view_matrix(entities, component_system);
}

auto entity_handler::cam(void) -> camera &
{
	return entity_camera;
}

auto entity_handler::create_component_system(void) -> void
{
	component_system.add_system<basic_key_control>(3);
	component_system.add_system<mouse_control>(3);
	component_system.add_system<height>(3);
}

/* 	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 vel;
	f32 speed;
	f32 size;
	*/

auto entity_handler::create_main_player(input_handler & ih) -> void
{
	entity new_entity;
	auto & data = new_entity.get_data();
	data.pos = glm::vec3(0, 10.0f, 0.0f);
	data.dir = glm::vec3(1, 0.00, 0.00);
	data.vel = glm::vec3(0);
	data.speed = 15;
	i32 at = entities.add(new_entity);
	bound_entity = at;
	component_system.add_component<height>(new_entity, at, height{ 1.0f });
	component_system.add_component<basic_key_control>(new_entity, at, ih);
	entity_camera.bind_entity(at, entities);

	component_system.add_component<mouse_control>(new_entity, at, ih, entity_camera);
}