#include "panel.h"

panel::panel(gui_cache const & cache, texture * diffuse)
{
	vertices.push_back(cache);

	this->diffuse = diffuse;
}