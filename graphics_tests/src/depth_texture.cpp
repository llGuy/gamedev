#include "depth_texture.h"

depth_texture::depth_texture(int32_t w, int32_t h)
	: width(w), height(h)
{
}

auto depth_texture::create(void) -> void
{
	glGenTextures(1, &id);
	bind(GL_TEXTURE_2D);
	fill(GL_TEXTURE_2D, GL_DEPTH_COMPONENT16, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	int_param(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	int_param(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_COMPARE_R_TO_TEXTURE);
}

auto depth_texture::set_dimensions(int32_t w, int32_t h) -> void
{
	width = w;
	height = h;
}