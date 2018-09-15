#pragma once

#include <glm/glm.hpp>
#include "types.h"

class quaternion
{
private:
	f32 x, y, z, w;
public:
	quaternion(glm::vec4 const & n, f32 a)
	{
		/* converted to radians */
		a = glm::radians(a);

		w = cos(a / 2.0f);
		x = n.x * sin(a / 2.0f);
		y = n.y * sin(a / 2.0f);
		z = n.z * sin(a / 2.0f);
	}
};