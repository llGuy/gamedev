#include "framebuffer.h"
#include "texture.h"
#include "renderbuffer.h"

auto unbind_all_framebuffers(i32 w, i32 h) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
}

auto framebuffer::create(i32 width, i32 height) -> u32
{
	glGenFramebuffers(1, &id);
	w = width;
	h = height;
	return id;
}

auto framebuffer::destroy(void) -> void { /* ... */ }

auto framebuffer::bind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, w, h);
}

auto framebuffer::unbind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto framebuffer::attach(texture & tex, u32 component, u32 level) -> void
{
	glFramebufferTexture(GL_FRAMEBUFFER, component, tex.id, level);
}

auto framebuffer::select_color_buffer(GLenum point) -> void
{
	glDrawBuffer(point);
}

auto framebuffer::framebuffer_status(void) -> bool
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

auto framebuffer::attach(renderbuffer & rnd, u32 component) -> void
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, component, GL_RENDERBUFFER, rnd.id);
}

auto framebuffer::clean_up(void) -> void
{
	glDeleteFramebuffers(1, &id);
}

auto framebuffer::resolve(framebuffer & other) -> void
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other.id);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, id);

	glBlitFramebuffer(0, 0, w, h, 0, 0, other.width(), other.height(), 
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}