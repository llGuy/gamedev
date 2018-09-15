#include "renderbuffer.h"

auto renderbuffer::create(void) -> u32
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

auto renderbuffer::set_storage(GLenum component, u32 w, u32 h) -> void
{
	glRenderbufferStorage(GL_RENDERBUFFER, component, w, h);
}

auto create_depth_buffer(renderbuffer & r, int32_t w, int32_t h) -> void
{
	r.create();
	r.bind();
	r.set_storage(GL_DEPTH_COMPONENT, w, h);
}