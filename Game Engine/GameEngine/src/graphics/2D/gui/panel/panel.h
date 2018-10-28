#pragma once

#include "../gui.h"

class panel : public gui
{
public:
	panel(void) = default;

	auto init(gui_cache const & cache, texture * diffuse) -> void;
};