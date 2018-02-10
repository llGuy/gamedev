#ifndef MOUSE_HEADER
#define MOUSE_HEADER

#include <unordered_map>
#include <functional>

struct GLFWwindow;

class MouseEvents
{
public:
	explicit MouseEvents(void) = default;
private:
	std::unordered_map<unsigned int, std::function<void(GLFWwindow*)>> m_events;
};

#endif