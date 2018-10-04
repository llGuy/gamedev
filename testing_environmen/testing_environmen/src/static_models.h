#pragma once

#include "types.h"
#include "noise_generator.h"

/* responsible for creating shape vertices and indices */
/* the color or texture or normals are up to the use to create */
template <typename T, typename = void> struct shape;

template <> struct shape <struct cube>
{
	f32 rad;

	shape(f32 radius)
		: rad(radius)
	{
	}

	auto operator()(model_handler & handler, model_data & data, model_instance index) const -> void;
private:
	auto create_vertices(void) const -> std::vector<glm::vec3>
	{
		return std::vector<glm::vec3> 
		{
			glm::vec3(-rad, -rad, +rad),
			glm::vec3(+rad, -rad, +rad),
			glm::vec3(+rad, +rad, +rad),
			glm::vec3(-rad, +rad, +rad),

			glm::vec3(-rad, -rad, -rad),
			glm::vec3(+rad, -rad, -rad),
			glm::vec3(+rad, +rad, -rad),
			glm::vec3(-rad, +rad, -rad)
		};
	}

	auto create_indices(void) const -> std::vector<u32>
	{
		return std::vector<u32>
		{
			// front
			0, 1, 2,
				2, 3, 0,
				// right
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// left
				4, 0, 3,
				3, 7, 4,
				// bottom
				5, 4, 1,
				0, 1, 4,
				// top
				2, 3, 6,
				7, 6, 3,
		};
	}
};