#ifndef _SHADOW_H_
#define _SHADOW_H_

#include <glm/glm.hpp>

#include "framebuffer.h"
#include "texture.h"
#include "program.h"

class shadows
{
public:
	shadows(void) = default;

	auto create(void) -> void;

	auto shaders(void) -> program &;
	auto fbo(void) -> framebuffer &;
	auto tex(void) -> texture &;

	auto bias(void) -> glm::mat4 &;
	auto depth_projection(void) -> glm::mat4 &;
private:
	auto create_fbo(void) -> void;
	auto create_program(void) -> void;
	auto create_projection(void) -> void;
private:
	program depth_shader;
	texture depth_texture;
	texture color_texture;
	framebuffer depth_framebuffer;

	glm::mat4 ortho_projection;
	glm::mat4 depth_bias;
};

#endif