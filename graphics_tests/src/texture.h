#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <GL/glew.h>

extern auto unbind_all_textures(void) -> void;

class texture
{
public:
	friend class framebuffer;

	texture(void) = default;

	virtual auto create(void) -> void;

	auto bind(uint32_t slot = -1) -> void;
	auto fill(GLenum internal_format, uint32_t w, uint32_t h, GLenum format, GLenum type, void const * data) -> void;

	auto enable_mipmap(void) -> void;
	auto float_param(GLenum mode, float factor) -> void;
	auto int_param(GLenum mode, GLenum factor) -> void;
protected:
	uint32_t id;
};

#endif
