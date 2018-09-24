#include <GL/glew.h>

#include "entity.h"
#include "program.h"
#include "ecs/graphics.h"
#include "ecs/physics.h"
#include "ecs/model_matrix.h"
#include "ecs/depth_render.h"
#include "ecs/basic_components.h"
#include "ecs/basic_key_control.h"
#include "ecs/little_gun.h"
#include "ecs/mouse_control.h"

entity_handler::entity_handler(void)
{
}

auto entity_handler::create(input_handler & ih, program & d, renderable & m, tracer_handler & th, puff_handler & ph) -> void
{
	create_component_system();
	create_main_player(ih, d, m, th, ph);
}

auto entity_handler::update(f32 td) -> void
{
	component_system.update_except<graphics, depth>(td, entities);
	entity_camera.update_view_matrix(entities, component_system);
}

auto entity_handler::cam(void) -> camera &
{
	return entity_camera;
}

auto entity_handler::create_component_system(void) -> void
{
	component_system.add_system<basic_key_control>(3);
	component_system.add_system<model_matrix>(10);
	component_system.add_system<mouse_control>(3);
	component_system.add_system<graphics>(10);
	component_system.add_system<physics>(1);
	component_system.add_system<height>(10);
	component_system.add_system<little_gun>(1);
	component_system.add_system<depth>(10);
}

auto entity_handler::add_entity(glm::vec3 const & p, glm::vec3 const & d, 
	renderable & model, program & color, program & dpth, glm::vec3 const & scale) -> void
{
	entity new_entity;
	auto & data = new_entity.get_data();

	data.pos = p;
	data.dir = d;
	data.vel = glm::vec3(0);
	data.size = scale;

	i32 at = entities.add(new_entity);
	
	component_system.add_component<height>(new_entity, at, height{2.f});
	component_system.add_component<model_matrix>(new_entity, at);
	component_system.add_component<graphics>(new_entity, at, color, model);
	component_system.add_component<depth>(new_entity, at, dpth, model);
}

auto entity_handler::create_main_player(input_handler & ih, program & dpth, renderable & model, tracer_handler & th, puff_handler & ph) -> void
{
	entity new_entity;
	auto & data = new_entity.get_data();
	data.pos = glm::vec3(3, 0.0f, 3.0f);
	data.dir = glm::vec3(1, 0.00, 0.00);
	data.vel = glm::vec3(0);
	data.speed = 15;
	data.size = glm::vec3(1);

	i32 at = entities.add(new_entity);
	bound_entity = at;
	component_system.add_component<height>(new_entity, at, height{1.2f });
	component_system.add_component<basic_key_control>(new_entity, at, ih);
	component_system.add_component<model_matrix>(new_entity, at);
	entity_camera.bind_entity(at, entities);

	component_system.add_component<mouse_control>(new_entity, at, ih, entity_camera);
	component_system.add_component<depth>(new_entity, at, dpth, model);
	component_system.add_component<physics>(new_entity, at);
	component_system.add_component<little_gun>(new_entity, at, ih, th, ph);
}