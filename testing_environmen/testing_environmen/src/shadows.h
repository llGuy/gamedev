#pragma once

#include "program.h"
#include "texture.h"
#include "detail.h"
#include "framebuffer.h"
#include <glm/gtx/transform.hpp>

class shadow_handler
{
private:
	i32 const map_size = 2048 * 2;

	framebuffer depth_fbo;
	texture depth_map;
	program shaders;

	f32 min_x, min_y, min_z;
	f32 max_x, max_y, max_z;

	f32 far_height, far_width,
		near_height, near_width;

	/* ls = light_space */
	glm::vec4 up_ls;
	glm::vec4 right_ls;
	glm::mat4 light_view_matrix;
	glm::mat4 projection_matrix;
public:
	auto create(i32 screen_w, i32 screen_h) -> void
	{
		create_fbo();
		unbind_all_framebuffers(screen_w, screen_h);
		create_shaders();
		calculate_light_view_matrix();
	}
private:
	auto create_fbo(void) -> void
	{
		depth_fbo.create(map_size, map_size);
		depth_fbo.bind();

		/* create texture */
		depth_map.create();
		depth_map.bind(GL_TEXTURE_2D);
		depth_map.fill(GL_TEXTURE_2D, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, map_size, map_size);
		depth_map.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		depth_map.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		depth_map.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		depth_map.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		depth_fbo.attach(depth_map, GL_DEPTH_ATTACHMENT, 0);
		depth_fbo.select_color_buffer(GL_NONE);
	}
	auto create_shaders(void) -> void
	{
		shaders.create_shader(GL_VERTEX_SHADER, "shadow/shadow_vsh.shader");
		shaders.create_shader(GL_FRAGMENT_SHADER, "shadow/shadow_fsh.shader");
		shaders.link_shaders("vertex_position");
		shaders.get_uniform_locations("mvp");
	}
	auto calculate_light_view_matrix(void) -> void
	{
		light_view_matrix = glm::lookAt(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 inverse = glm::inverse(light_view_matrix);
		right_ls = inverse * glm::vec4(1, 0, 0, 1);
		up_ls = inverse * glm::vec4(0, 1, 0, 1);
	}
public:
	auto get_shaders(void) -> program & { return shaders; };
	auto get_fbo(void) -> framebuffer & { return depth_fbo; };
	auto get_depth_map(void) -> texture & { return depth_map; };
	
	auto light_view(void) -> glm::mat4 & { return light_view_matrix; };
	auto orthographic_projection(void) -> glm::mat4 & { return projection_matrix; };

	auto update(glm::vec3 const & pos, glm::vec3 const & dir) -> void
	{
		f32 far = 100.0f;
		f32 near = 0.1f;

		/* calculating frustum corners in world space */
		far_width = (f32)(2.0f * far * tan(glm::radians(60.0f)));
		near_width = (f32)(2.0f * near * tan(glm::radians(60.0f)));
		far_height = far_width / (1440.0f / 850.0f);
		near_height = near_width / (1440.0f / 850.0f);

		glm::vec3 right_view = glm::normalize(glm::cross(dir, detail::up));
		glm::vec3 up_view = glm::normalize(glm::cross(dir, right_view));

		glm::vec3 far_left_top = pos + dir * far - right_view * (far_width / 2.0f) + up_view * (far_height / 2.0f);
		glm::vec3 far_left_bot = pos + dir * far - right_view * (far_width / 2.0f) - up_view * (far_height / 2.0f);

		glm::vec3 far_right_top = pos + dir * far + right_view * (far_width / 2.0f) + up_view * (far_height / 2.0f);
		glm::vec3 far_right_bot = pos + dir * far + right_view * (far_width / 2.0f) - up_view * (far_height / 2.0f);

		glm::vec3 near_right_top = pos + dir * near + right_view * (near_width / 2.0f) + up_view * (near_height / 2.0f);
		glm::vec3 near_right_bot = pos + dir * near + right_view * (near_width / 2.0f) - up_view * (near_height / 2.0f);

		glm::vec3 near_left_top = pos + dir * near - right_view * (near_width / 2.0f) + up_view * (near_height / 2.0f);
		glm::vec3 near_left_bot = pos + dir * near - right_view * (near_width / 2.0f) - up_view * (near_height / 2.0f);

		glm::vec4 all[]{
			glm::vec4(far_left_top, 1.0f), glm::vec4(far_left_bot, 1.0f),
			glm::vec4(far_right_top, 1.0f), glm::vec4(far_right_bot, 1.0f),
			glm::vec4(near_right_top, 1.0f), glm::vec4(near_right_bot, 1.0f),
			glm::vec4(near_left_top, 1.0f), glm::vec4(near_right_top, 1.0f)
		};

		calculate_orthogonal_projection(all);
	}

	auto calculate_orthogonal_projection(glm::vec4 * world_space_corners) -> void
	{
		/* transform to light space */
		for (u32 i = 0; i < 8; ++i)
			world_space_corners[i] = light_view_matrix * world_space_corners[i];

		auto find_impl = [&](auto comp_func, glm::vec4 * all, u32 index) -> f32
		{
			f32 res = all[0][index];
			for (u32 i = 1; i < 8; ++i)
				if (comp_func(all[i][index], res)) res = all[i][index];
			return res;
		};

		auto smallest = [&](f32 a, f32 b) -> bool { return a < b; };
		auto biggest = [&](f32 a, f32 b) -> bool { return a > b; };

		min_x = find_impl(smallest, world_space_corners, 0);
		min_y = find_impl(smallest, world_space_corners, 1);
		min_z = find_impl(smallest, world_space_corners, 2);

		max_x = find_impl(biggest, world_space_corners, 0);
		max_y = find_impl(biggest, world_space_corners, 1);
		max_z = find_impl(biggest, world_space_corners, 2);

		projection_matrix = glm::ortho<f32>(min_x, max_x, min_y, max_y, min_z, max_z);
	}
};