#pragma once

#include "../types.h"
#include <glm/glm.hpp>

namespace {

	class ray
	{
	public:
		ray(glm::vec3 const & pos, glm::vec3 const & dir, float scale, float max)
			: current_pos(pos), direction(dir * scale), max_distance(max), interval(0)
		{
			glm::vec3 start = current_pos;
			current_pos += direction;
			distance_per_interval = glm::distance(start, current_pos);
			++interval;
		}

		inline auto current_position(void) -> glm::vec3 const & { return current_pos; }
		inline auto distance_covered(void) -> f32 { return interval * distance_per_interval; }
		inline auto distance_max(void) -> f32 { return max_distance; }
		auto extend(void) -> void 
		{
			current_pos += direction;
			++interval;
		}
	private:
		u32 interval;
		f32 max_distance;
		f32 distance_per_interval;
		glm::vec3 current_pos;
		glm::vec3 direction;
	};

}