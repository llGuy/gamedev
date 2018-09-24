#pragma once

#include "../types.h"
#include "../detail.h"
#include "../texture.h"
#include "../program.h"
#include "../framebuffer.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class shadow_handler
{
private:
	static constexpr i32 shadow_map_size = 4096;

	/* dimensions of the frustum */
	f32 far_width, far_height;
	f32 near_width, near_height;

	/* f = far, n = near, l = left, r = right, t = top, b = bottom */
	enum ortho_corner : i32
	{
		flt, flb,
		frt, frb,
		nlt, nlb,
		nrt, nrb
	};

	/* dimensions of the orthogonal projection */
	f32 x_min, y_min, z_min;
	f32 x_max, y_max, z_max;

	/* projection matrices */
	glm::mat4 light_view_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 shadow_bias;

	/* opengl objects */
	framebuffer depth_fbo;
	texture depth_map;
	program shaders;
public:
	shadow_handler(void) = default;
	shadow_handler(shadow_handler const &) = delete;
public:
	auto get_projection(void) -> glm::mat4 &;
	auto get_depth_map(void) -> texture &;
	auto get_fbo(void) -> framebuffer &;
	auto get_shaders(void) -> program &;
	auto get_light_view(void) -> glm::mat4 &;
	auto get_shadow_bias(void) -> glm::mat4 &;
public:
	auto create(glm::vec3 const & light_pos) -> void;
	/* update method calculates the projection matrix in the light view space for the 
	   "light's view", this needs the parameters of the frustum for the perspective matrix*/
	auto update(f32 far, f32 near, f32 aspect, f32 fov, glm::vec3 const & pos, glm::vec3 const & dir) -> void;
	/* updates when the light position changes */
	auto update_light_view(glm::vec3 const & light_pos) -> void;
private:
	/* methods used in the update function */
	/* calculates the dimensions of the view frustum of the perspective matrix of the scene */
	auto calculate_frustum_dimensions(f32 far, f32 near, f32 aspect, f32 fov) -> void;
	/* calculates the corners of the orthographics projection matrix's box */
	auto calculate_ortho_corners(glm::vec3 const & pos, glm::vec3 const & dir, f32 far, f32 near, std::array<glm::vec4, 8> & corners) -> void;

	auto find_min_max_values(std::array<glm::vec4, 8> & corners) -> void;
private:
	auto create_fbo(void) -> void;

	auto create_depth_texture(void) -> void;

	auto create_shaders(void) -> void;

	auto create_light_view_matrix(glm::vec3 const & light_dir) -> void;

	auto create_shadow_bias_matrix(void) -> void;
};