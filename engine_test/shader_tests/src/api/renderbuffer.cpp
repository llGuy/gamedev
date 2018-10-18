#include "renderbuffer.h"

auto renderbuffer::create(void)-> u32
{
	glGenRenderbuffers(1, &id);
	return id;
}

auto renderbuffer::clean_up(void) -> void
{
	glDeleteRenderbuffers(1, &id);
}

auto renderbuffer::bind(void) -> void
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}

auto renderbuffer::set_storage_ms(i32 samples, GLenum format, i32 w, i32 h) -> void
{
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, w, h);
}

auto renderbuffer::set_storage(GLenum component, u32 w, u32 h) -> void
{
	glRenderbufferStorage(GL_RENDERBUFFER, component, w, h);
}