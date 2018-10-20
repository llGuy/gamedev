#include "framebuffer.h"

framebuffer::framebuffer(void)
	: id(0)
{
}

auto framebuffer::create(i32 width, i32 height) -> u32
{
	glGenFramebuffers(1, &id);
	w = width;
	h = height;
	return id;
}

auto framebuffer::clean_up(void) -> void
{
	glDeleteFramebuffers(1, &id);
}

auto framebuffer::bind(GLenum point) -> void
{
	glBindFramebuffer(point, id);
	glViewport(0, 0, w, h);
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

auto framebuffer::add_texture(u32 id) -> void
{
	outs.push_back(id);
}

auto framebuffer::attach(texture & obj, u32 attachment, u32 level) -> void
{
	glFramebufferTexture(GL_FRAMEBUFFER, attachment, obj.id, level);
}

auto framebuffer::attach(renderbuffer & obj, u32 attachment) -> void
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, obj.id);
}

auto framebuffer::select_color_buffer(u32 point) -> void
{
	glDrawBuffer(point);
}

auto framebuffer::framebuffer_status(void) -> bool
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

auto framebuffer::width(void) -> i32 &
{
	return w;
}

auto framebuffer::height(void) -> i32 &
{
	return h;
}