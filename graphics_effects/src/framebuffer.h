#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <GL/glew.h>
#include <stdint.h>

class texture;

class framebuffer
{
public:
	framebuffer(void) = default;

	auto create(void) -> uint32_t;
	auto destroy(void) -> void;
	auto bind(void) -> void;
	auto unbind(void) -> void;

    auto attach_texture(texture & obj, uint32_t component, uint32_t level) -> void;

	auto select_color_buffer(uint32_t point) -> void;
	auto framebuffer_status(void) -> bool;
private:
	uint32_t id;
};

#endif
