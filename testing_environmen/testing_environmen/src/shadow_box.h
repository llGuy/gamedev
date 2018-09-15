#pragma once

#include "types.h"
#include "camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

class shadow_box
{
private:
	f32 const offset = 10;
	f32 shadow_distance = 100;

	f32 minx, maxx;
	f32 miny, maxy;
	f32 minz, maxz;

	glm::mat4 light_view_matrix;
	camera * cam;

	f32 far_height, far_width, near_height, near_width;
public:
	shadow_box(camera & cam)
		: cam(&cam)
	{
	}
	auto get_center(void) -> glm::vec3
	{
		f32 x = (minx + maxx) / 2.0f;
		f32 y = (miny + maxy) / 2.0f;
		f32 z = (minz + maxz) / 2.0f;

		glm::vec4 center(x, y, z, 1.0f);
		glm::mat4 inverted_light = glm::inverse(light_view_matrix);
		return inverted_light * center;
	}
	auto calculate_widths_and_heights(void)
	{
		/* so far makes sense */
		far_width = (f32)(shadow_distance * tan(glm::radians(60.0f)));
		near_width = (f32)(0.1f * tan(glm::radians(60.0f)));
		far_height = far_width / (1440.0f / 850.0f);
		near_height = near_width / (1440.0f / 850.0f);
	}
	auto calculate_frustum_vertices(glm::mat4 const & rotation, glm::vec3 const & forward, 
		glm::vec3 const & center_near, glm::vec3 const & center_far, glm::vec4 * results) -> void
	{

	}
};