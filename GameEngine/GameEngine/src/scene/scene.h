#pragma once

#include "game_object.h"
#include "component/component.h"

class scene
{
private:
	game_object_component_system components;

	/* vector of game objects */
	vector_dyndel<game_object> objects;
public:
	auto init(void) -> void;

	auto init_game_object(game_object_data const & data) -> game_object &;

	auto operator[](u32 index) -> game_object &;

	auto update(f32 td) -> void;
};