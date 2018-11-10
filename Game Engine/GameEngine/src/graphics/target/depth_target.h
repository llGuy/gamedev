#pragma once

#include "target.h"

/* for shadows */
class depth_target : public target
{
private:

public:
	auto init(texture_handler & textures) -> void override;
};