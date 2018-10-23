#pragma once

#include "game_object.h"

class scene
{
private:

	game_object_component_system components;

private:

	/* vector of objects */
	vector_dyndel<game_object> objects;

public:

	auto init_systems(void) -> void;

	auto submit_game_object(game_object const & object) -> void;
	auto operator[](u32 index) -> game_object &;

	auto update_render(f32 td) -> void;
	auto update(f32 td) -> void;

};