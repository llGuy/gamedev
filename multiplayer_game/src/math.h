#ifndef _MATH_HEADER_
#define _MATH_HEADER_

#include <glm/glm.hpp>

namespace mulgame {

	extern float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
	extern const bool Equf(float a, float b);

}

#endif
