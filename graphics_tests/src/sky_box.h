#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include "texture.h"
#include "resources.h"

class sky_box
{
public:
	sky_box(void);
	auto create(resource_handler & rh) -> void;
private:
	texture sky_box_texture;
};

#endif