#pragma once

#include "camera.h"
#include "game_object.h"
#include "component/component.h"

class scene
{
private:
	game_object_component_system components;

	/* vector of game objects */
	vector_dyndel<game_object> objects;
	std::unordered_map<std::string, u32> index_map;

	camera scene_camera;
public:
	auto init(void) -> void;

	auto init_game_object(game_object_data const & data) -> game_object &;

	auto operator[](u32 index) -> game_object &;

	auto get_game_object(std::string const & name) -> game_object &;

	auto update(f32 td) -> void;

	auto get_scene_camera(void) -> camera &;

	auto bind_camera_to_object(game_object & subject) -> void;
};