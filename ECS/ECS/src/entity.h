#pragma once

#include "types.h"
#include <vector>

struct handle
{
	i32 type : 10;
	i32 index : 22;
};

struct entity
{
	std::vector<handle> components;
};