#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <GL/glew.h>
#include "window.h"
#include "timer.h"
#include "scene_game_state.h"

class application
{
public:
	application(int32_t w, int32_t h);

	auto init(void) -> void;
	auto init_window(void) -> void;
	auto render(void) -> void;
	auto update(void) -> void;
	auto running(void) -> bool;
	auto destroy(void) -> void;
private:
	timer time_handler;
	window appl_window;
	std::unique_ptr<game_state> state;
};

#endif