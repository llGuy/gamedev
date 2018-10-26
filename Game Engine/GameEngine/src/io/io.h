#pragma once

#include <string>
#include "../utils/types.h"

extern auto extract_file(std::string const & dir)->std::string;

struct image
{
	void * data;
	i32 w, h;
};

extern auto extract_png(std::string const & dir, bool flipvert = false)->image;