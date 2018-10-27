#pragma once

#include "../gui.h"

class panel : public gui
{
public:
	panel(gui_cache const & cache, texture * diffuse);
};