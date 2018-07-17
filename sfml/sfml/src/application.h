#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "grid.h"

class application
{
public:
	application(int32_t w, int32_t h);

	auto init(void) -> void;
	auto update(void) -> void;
	auto running(void) -> bool;
private:
	default_grid grid;
};

#endif
