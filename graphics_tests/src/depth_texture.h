#ifndef _DEPTH_TEXTURE_H_
#define _DEPTH_TEXTURE_H_

#include "texture.h"

class depth_texture
	: public texture
{
public:
	depth_texture(int32_t w, int32_t h);

	auto create(void) -> void override;
private:
	int32_t width;
	int32_t height;
};

#endif