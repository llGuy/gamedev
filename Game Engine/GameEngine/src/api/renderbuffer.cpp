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

auto create_depth_renderbuffer(renderbuffer & rnd, i32 w, i32 h) -> void
{
	rnd.create();
	rnd.bind();
	rnd.set_storage(GL_DEPTH_COMPONENT, w, h);
}

auto create_depth_renderbuffer_ms(renderbuffer & rnd, i32 samples, i32 w, i32 h) -> void
{
	rnd.create();
	rnd.bind();
	rnd.set_storage_ms(samples, GL_DEPTH_COMPONENT, w, h);
}

auto create_color_renderbuffer(renderbuffer & rnd, i32 w, i32 h) -> void
{
	rnd.create();
	rnd.bind();
	rnd.set_storage(GL_RGBA, w, h);
}

auto create_color_renderbuffer_ms(renderbuffer & rnd, i32 samples, i32 w, i32 h) -> void
{
	rnd.create();
	rnd.bind();
	rnd.set_storage_ms(samples, GL_RGBA, w, h);
}