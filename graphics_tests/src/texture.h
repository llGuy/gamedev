#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <GL/glew.h>

extern auto unbind_all_textures(GLenum target) -> void;

class texture
{
public:
	friend class framebuffer;

	virtual auto create(void) -> void;

	auto bind(GLenum target, uint32_t slot = -1) -> void;
	auto fill(GLenum target,GLenum internal_format, uint32_t w, uint32_t h, 
		GLenum format, GLenum type, void const * data) -> void;

	auto enable_mipmap(GLenum target) -> void;
	auto float_param(GLenum target, GLenum mode, float factor) -> void;
	auto int_param(GLenum target, GLenum mode, GLenum factor) -> void;
protected:
	uint32_t id;
};

#endif
