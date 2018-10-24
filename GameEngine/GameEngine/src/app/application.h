#pragma once

#include "../time/timer.h"
#include "../scene/scene.h"
#include "../window/window.h"

class application
{
private:
	bool is_running;
	window display;
	scene world;
	timer time_handler;
public:
	application(void);

	auto init(void) -> void;
	auto update(void) -> void;
	auto render(void) -> void;
	auto running(void) -> bool;
	auto clean_up(void) -> void;
private:
	auto init_game_objects(void) -> void;
};