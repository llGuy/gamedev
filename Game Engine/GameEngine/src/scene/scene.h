#pragma once

#include "camera.h"
#include "loader.h"
#include "game_object.h"

class scene
{
private:
	friend class scene_loader;

	game_object_component_system components;

	/* vector of game objects */
	vector_dyndel<game_object> objects;
	std::unordered_map<std::string, u32> index_map;

	camera scene_camera;

	scene_loader loader;
public:
	auto init(input_handler & inputs, model_handler & models, material_handler & materials) -> void;

	auto init_game_object(game_object_data const & data) -> game_object &;

	auto operator[](u32 index) -> game_object &;

	auto get_game_object(std::string const & name) -> game_object &;

	auto update(f32 td) -> void;

	auto get_scene_camera(void) -> camera &;

	auto bind_camera_to_object(game_object & subject) -> void;

	auto load_from_file(void) -> void;
};