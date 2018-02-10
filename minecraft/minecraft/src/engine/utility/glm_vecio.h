#ifndef GLM_VECIO_HEADER
#define GLM_VECIO_HEADER

#include <iostream>
#include <glm/glm.hpp>

extern std::ostream& operator<<(std::ostream& os, glm::vec2& v2);
extern std::ostream& operator<<(std::ostream& os, glm::vec3& v3);

#endif