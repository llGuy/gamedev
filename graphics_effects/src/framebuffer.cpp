#include "framebuffer.h"

auto framebuffer::create(void) -> uint32_t
{
	glGenFramebuffers(1, &id);
}

auto framebuffer::destroy(void) -> void
{

}

auto framebuffer::bind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

auto framebuffer::unbind(void) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto framebuffer::attach_texture(texture & obj, uint32_t component, uint32_t level) -> void
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, component, obj.id, level);
}

auto framebuffer::select_color_buffer(uint32_t point) -> void
{
	glDrawBuffer(point);
}

auto framebuffer::framebuffer_status(void) -> bool
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
}
