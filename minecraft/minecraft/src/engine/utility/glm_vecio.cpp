#include <iostream>
#include <glm/glm.hpp>

std::ostream& operator<<(std::ostream& os, glm::vec2& v2)
{
	os << v2.x << " , " << v2.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, glm::vec3& v3)
{
	os << v3.x << " , " << v3.y << " , " << v3.z;
	return os;
}