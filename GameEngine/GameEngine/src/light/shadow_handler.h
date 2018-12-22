#pragma once

#include <glm/glm.hpp>
#include "../api/api.h"
#include "../utils/types.h"
#include "../utils/detail.h"
#include "../scene/camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../api/uniform_buffer.h"

struct shadow_uniform_block_data
{
	glm::mat4 shadow_bias;

	f32 transition_distance;
	f32 shadow_distance;
	f32 shadow_map_size;
	f32 pcf_count;
};

class shadow_handler
{
private:
	static constexpr u32 shadow_map_size = 4096;

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

	std::array<glm::vec4, 8> corners;

	/* projection matrices */
	glm::mat4 light_view_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 shadow_bias;

	glm::vec3 light_pos;
	glm::vec3 light_dir;

	camera shadow_camera;

	uniform_buffer shadow_uniform_block;
	texture * shadow_map;
public:
	shadow_handler(void)
		: shadow_uniform_block(SHADOW_BLOCK_INDEX)
	{
	}
	shadow_handler(shadow_handler const &) = delete;
public:
	auto get_projection(void) -> glm::mat4 & { return projection_matrix; }
	auto get_light_view(void) -> glm::mat4 & { return light_view_matrix; }
	auto get_shadow_bias(void) -> glm::mat4 & { return shadow_bias; }
	auto get_shadow_camera(void) -> camera & { return shadow_camera; }

	static constexpr auto get_shadow_map_size(void) -> u32 { return shadow_map_size; };
public:
	auto create(glm::vec3 const & light_pos, texture * shadow_map) -> void
	{
		this->shadow_map = shadow_map;

		create_shadow_bias_matrix();
		create_light_view_matrix(light_dir = glm::normalize(glm::vec3(light_pos.x, -light_pos.y, light_pos.z)));

		/* create uniform buffer */
		shadow_uniform_block.create();
		shadow_uniform_block.bind(GL_UNIFORM_BUFFER);
		shadow_uniform_block.fill<void>(sizeof shadow_uniform_block_data, nullptr, GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);

		auto ptr = shadow_uniform_block.map(GL_UNIFORM_BUFFER, GL_READ_ONLY);

		shadow_uniform_block_data data;
		data.shadow_distance = 100.0f;
		data.shadow_map_size = 4096.0f;
		data.transition_distance = 10.0f;
		data.pcf_count = 2.0f;
		data.shadow_bias = shadow_bias;

		*((shadow_uniform_block_data *)ptr) = data;

		unmap_buffers(GL_UNIFORM_BUFFER);
	}

	auto prepare_shader(glsl_program & program) -> void
	{
		program.bind_uniform_block(shadow_uniform_block, "shadow_data");
	}

	auto update(f32 far, f32 near, f32 aspect, f32 fov, glm::vec3 const & pos, glm::vec3 const & dir) -> void
	{
		calculate_frustum_dimensions(far, near, aspect, fov);
		calculate_ortho_corners(pos, dir, far, near, corners);
		find_min_max_values(corners);
		projection_matrix = glm::ortho<f32>(x_min, x_max, y_min, y_max, z_min, z_max);

		shadow_camera.get_projection_matrix() = projection_matrix;
		shadow_camera.get_view_matrix() = light_view_matrix;

		glm::mat4 bias = shadow_bias * shadow_camera.get_projection_matrix() * shadow_camera.get_view_matrix();
		shadow_uniform_block.partial_fill<glm::mat4>(0, sizeof glm::mat4, &bias, GL_UNIFORM_BUFFER);
	}
	auto update_light_view(glm::vec3 const & light_pos) -> void
	{
		create_light_view_matrix(glm::normalize(glm::vec3(light_pos.x, -light_pos.y, light_pos.z)));
	}
	auto get_perspective_view(void) -> glm::mat4
	{
		glm::mat4 inverse_view = glm::inverse(light_view_matrix);

		glm::vec4 middle = corners[flb] + (corners[flt] - corners[flb]) / 2.0f;
		middle = middle + (corners[frt] - corners[flt]) / 2.0f;

		glm::vec3 middle_vert = glm::vec3(inverse_view * middle);

		return glm::lookAt(middle_vert, middle_vert + light_dir, detail::up);
	}
private:
	/* update methods */
	auto calculate_frustum_dimensions(f32 far, f32 near, f32 aspect, f32 fov) -> void
	{
		far_width = 2.0f * far * tan(glm::radians(fov));
		near_width = 2.0f * near * tan(glm::radians(fov));
		far_height = far_width / aspect;
		near_height = near_width / aspect;
	}
	auto calculate_ortho_corners(glm::vec3 const & pos, glm::vec3 const & dir, f32 far, f32 near, std::array<glm::vec4, 8> & corners) -> void
	{
		glm::vec3 right_view = glm::normalize(glm::cross(dir, detail::up));
		glm::vec3 up_view = glm::normalize(glm::cross(dir, right_view));

		f32 far_width_half = far_width / 2.0f;
		f32 near_width_half = near_width / 2.0f;
		f32 far_height_half = far_height / 2.0f;
		f32 near_height_half = near_height / 2.0f;

		/* in light view space */
		corners[flt] = light_view_matrix * glm::vec4(pos + dir * far - right_view * far_width_half + up_view * far_height_half, 1.0f);
		corners[flb] = light_view_matrix * glm::vec4(pos + dir * far - right_view * far_width_half - up_view * far_height_half, 1.0f);

		corners[frt] = light_view_matrix * glm::vec4(pos + dir * far + right_view * far_width_half + up_view * far_height_half, 1.0f);
		corners[frb] = light_view_matrix * glm::vec4(pos + dir * far + right_view * far_width_half - up_view * far_height_half, 1.0f);

		corners[nlt] = light_view_matrix * glm::vec4(pos + dir * near - right_view * near_width_half + up_view * near_height_half, 1.0f);
		corners[nlb] = light_view_matrix * glm::vec4(pos + dir * near - right_view * near_width_half - up_view * near_height_half, 1.0f);

		corners[nrt] = light_view_matrix * glm::vec4(pos + dir * near + right_view * near_width_half + up_view * near_height_half, 1.0f);
		corners[nrb] = light_view_matrix * glm::vec4(pos + dir * near + right_view * near_width_half - up_view * near_height_half, 1.0f);
	}
	auto find_min_max_values(std::array<glm::vec4, 8> & corners) -> void
	{
		x_min = x_max = corners[0].x;
		y_min = y_max = corners[0].y;
		z_min = z_max = corners[0].z;

		for (u32 i = 1; i < 8; ++i)
		{
			if (x_min > corners[i].x) x_min = corners[i].x;
			if (x_max < corners[i].x) x_max = corners[i].x;

			if (y_min > corners[i].y) y_min = corners[i].y;
			if (y_max < corners[i].y) y_max = corners[i].y;

			if (z_min > corners[i].z) z_min = corners[i].z;
			if (z_max < corners[i].z) z_max = corners[i].z;
		}
	}
private:
	auto create_light_view_matrix(glm::vec3 const & light_dir) -> void
	{
		light_view_matrix = glm::lookAt(glm::normalize(-light_dir), glm::vec3(0), detail::up);
	}
	auto create_shadow_bias_matrix(void) -> void
	{
		shadow_bias = glm::mat4
		(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);
	}
};