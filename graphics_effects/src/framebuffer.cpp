#include "framebuffer.h"
#include "texture.h"

auto framebuffer::create(void) -> uint32_t
{
	glGenFramebuffers(1, &id);
	return id;
}

auto framebuffer::destroy(void) -> void { /* ... */ }

auto framebuffer::bind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

auto framebuffer::unbind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto framebuffer::attach_texture(texture & tex, uint32_t component, uint32_t level) -> void
{
	glFramebufferTexture(GL_FRAMEBUFFER, component, tex.id, level);
}

auto framebuffer::select_color_buffer(uint32_t point) -> void
{
	glDrawBuffer(point);
}

auto framebuffer::framebuffer_status(void) -> bool
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}