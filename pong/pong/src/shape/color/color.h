#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <glm\glm.hpp>

constexpr float COLOR_PEAK = 1.0f;
constexpr float COLOR_DELTA = 0.2f;

enum color_t
{
	RED,

	GREEN,

	BLUE,

	YELLOW,

	MAGENTA
};

struct Color
{
	Color(color_t type)
	{
		switch (type)
		{
		case RED:
			m_colorFB = glm::vec3(COLOR_PEAK, 0.0f, 0.0f);
			m_colorPs = glm::vec3(COLOR_PEAK - COLOR_DELTA, 0.0f, 0.0f);
			m_colorSs = glm::vec3(COLOR_PEAK - COLOR_DELTA * 2, 0.0f, 0.0f);
			break;
		case GREEN:
			m_colorFB = glm::vec3(0.0f, COLOR_PEAK, 0.0f);
			m_colorPs = glm::vec3(0.0f, COLOR_PEAK - COLOR_DELTA, 0.0f);
			m_colorSs = glm::vec3(0.0f, COLOR_PEAK - COLOR_DELTA * 2, 0.0f);
			break;
		case BLUE:
			m_colorFB = glm::vec3(0.0f, 0.0f, COLOR_PEAK);
			m_colorPs = glm::vec3(0.0f, 0.0f, COLOR_PEAK - COLOR_DELTA);
			m_colorSs = glm::vec3(0.0f, 0.0f, COLOR_PEAK - COLOR_DELTA * 2);
			break;
		case YELLOW:
			m_colorFB = glm::vec3(COLOR_PEAK, COLOR_PEAK, 0.0f);
			m_colorPs = glm::vec3(COLOR_PEAK - COLOR_DELTA, COLOR_PEAK - COLOR_DELTA, 0.0f);
			m_colorSs = glm::vec3(COLOR_PEAK - COLOR_DELTA * 2, COLOR_PEAK - COLOR_DELTA * 2, 0.0f);
			break;
		case MAGENTA:
			m_colorFB = glm::vec3(COLOR_PEAK, 0.0f, COLOR_PEAK);
			m_colorPs = glm::vec3(COLOR_PEAK - COLOR_DELTA, 0.0f, COLOR_PEAK - COLOR_DELTA);
			m_colorSs = glm::vec3(COLOR_PEAK - COLOR_DELTA * 2, 0.0f, COLOR_PEAK - COLOR_DELTA * 2);
			break;
		}
	}
	glm::vec3 m_colorFB;		//front back
	glm::vec3 m_colorPs;		//poles
	glm::vec3 m_colorSs;		//sides
};

#endif