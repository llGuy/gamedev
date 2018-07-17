#include "renderbuffer.h"

auto renderbuffer::create(void) -> uint32_t
{
	glGenRenderbuffers(1, &id);
	return id;
}

auto renderbuffer::bind(void) -> void
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}

auto renderbuffer::destroy(void) -> void
{
	glDeleteRenderbuffers(1, &id);
}

auto renderbuffer::set_storage(GLenum component, uint32_t w, uint32_t h) -> void
{
	glRenderbufferStorage(GL_RENDERBUFFER, component, w, h);
}