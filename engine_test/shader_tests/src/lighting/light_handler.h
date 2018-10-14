#pragma once

#include "light_info.h"

/* may handle night-day stuff */
class light_handler
{
private:

	light_info light;

public:

	light_handler(void);

	inline auto light_position(void) -> glm::vec3 &
	{
		return light.light_position;
	}

	inline auto ambient_intensity(void) -> glm::vec3 &
	{
		return light.ambient_intensity;
	}

	inline auto diffuse_intensity(void) -> glm::vec3 &
	{
		return light.diffuse_intensity;
	}

	inline auto specular_intensity(void) -> glm::vec3 &
	{
		return light.specular_intensity;
	}
};
