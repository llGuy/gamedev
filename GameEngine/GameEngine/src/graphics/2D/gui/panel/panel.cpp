#include "panel.h"

auto panel::init(gui_cache const & cache, texture * diffuse) -> void
{
	vertices.push_back(cache);
	this->diffuse = diffuse;
}