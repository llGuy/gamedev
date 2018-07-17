#ifndef _RENDERBUFFER_H_
#define _RENDERBUFFER_H_

#include <stdint.h>
#include <GL/glew.h>

class renderbuffer
{
	friend class framebuffer;
public:
	renderbuffer(void) = default;

	auto create(void) -> uint32_t;
	auto bind(void) -> void;
	auto destroy(void) -> void;

	auto set_storage(GLenum component, uint32_t w, uint32_t h) -> void;
private:
	uint32_t id;
};

#endif