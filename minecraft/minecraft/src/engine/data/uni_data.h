#ifndef UNI_DATA_HEADER
#define UNI_DATA_HEADER

#include <glm/glm.hpp>

namespace minecraft
{
	namespace data
	{
		struct CUData
		{
			glm::mat4 projectionMatrix;
			glm::mat4 modelMatrix;
			glm::mat4 viewMatrix;
			glm::vec3 lightPosition;
			glm::vec3 eyePosition;
			glm::vec3 skyColor;

			float liquidMeshBlockType;
		};

		struct CUDataLocs
		{
			uint32_t projectionMatrixLocation;
			uint32_t modelMatrixLocation;
			uint32_t viewMatrixLocation;
			uint32_t lightPositionLocation;
			uint32_t eyePositionLocation;
			uint32_t skyColorLocation;

			// for liquid mesh
			uint32_t liquidBlockTypeLocation;
		};
	}
}

#endif