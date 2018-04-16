#ifndef CONFIGS_HEADER
#define CONFIGS_HEADER
#include <glm/glm.hpp>

namespace dawn { namespace configs {

		static const glm::vec3 DEFAULT_PLAYER_POSITION = glm::vec3(0.5f, 20.0f, 0.5f);
		static const glm::vec3 DEFAULT_PLAYER_DIRECTION = glm::vec3(-0.5f, -1.0f, -0.5f);
		static const float FOV_DEGREES = 70.0f;
		static const float MOUSE_SENSITIVITY = 50.0f;
		static const float RENDER_DISTANCE = 500.0f;

	}
}

#endif