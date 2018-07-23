#ifndef _WATER_H_
#define _WATER_H_

#include "program.h"
#include "depth_texture.h"
#include "quad.h"
#include "renderbuffer.h"
#include "texture.h"
#include "framebuffer.h"

class water
{
public:
	water(void) = default;

	auto create(resource_handler &) -> void;
	auto prepare(glm::mat4 & proj, glm::mat4 & view, glm::vec3 & cam_pos, float elapsed) -> void;

	auto quad(void)->quad_3D &;
	auto bind_refl(void) -> void;
	auto bind_refr(void) -> void;
	auto unbind_framebuffers(int32_t w, int32_t h) -> void;

	// getters
	auto refl_texture(void) -> texture &;
	auto refr_texture(void) -> texture &;
	auto refr_depth_texture(void) -> texture &;
private:
	auto bind_framebuffer(framebuffer & fbo, int32_t w, int32_t h) -> void;

	auto create_reflection_fbo(int32_t, int32_t) -> void;
	auto create_refraction_fbo(int32_t, int32_t) -> void;
	auto create_color_texture_attachment(texture & t, framebuffer & fbo, int32_t, int32_t) -> void;
	auto create_depth_texture_attachment(depth_texture & t, framebuffer & fbo, int32_t, int32_t) -> void;
	auto create_depth_buffer_attachment(renderbuffer & r, framebuffer & fbo, int32_t w, int32_t h) -> void;
private:
	static constexpr int32_t reflection_width = 320;
	static constexpr int32_t refraction_width = 1280;
	static constexpr int32_t reflection_height = 180;
	static constexpr int32_t refraction_height = 720;

	quad_3D water_quad;

	framebuffer water_refraction_fbo;
	framebuffer water_reflection_fbo;

	texture refl_color;
	renderbuffer refl_depth;
	texture refr_color;
	depth_texture refr_depth;

	program water_shaders;
	texture dudv_texture;
	texture normal_map_texture;
};

#endif