#ifndef UDATA_HEADER
#define UDATA_HEADER

#include <glm/glm.hpp>

namespace dawn {

	struct UniformData
	{
		glm::mat4 projection;
		glm::mat4 model;
		glm::mat4 view;
	};

	struct UniformDataLoc
	{
		uint32_t projectionLoc;
		uint32_t modelLoc;
		uint32_t viewLoc;
	};

}

#endif