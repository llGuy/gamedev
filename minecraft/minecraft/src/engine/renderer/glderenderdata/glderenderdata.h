#ifndef GLDERENDERDATA_HEADER
#define GLDERENDERDATA_HEADER

#include <stdint.h>

#include "../../buffer/vao.h"
 
namespace minecraft
{
	namespace rnd
	{
		/*struct GLDrawElementsRenderData
		{
			glm::mat4 modelMatrix;
			uint32_t bufferData;
			void* offsetIndices;
			VAO* vao;
			Texture* t;
			uint32_t count;
		};*/
		struct GLDrawElementsRenderData 
		{
			glm::mat4 modelMatrix;
			uint32_t bufferData;
			void* offsetIndices;
			VAO* vao;
			Texture* t;
			uint32_t count;
		};
		struct GLDrawElementsRenderDataAll
		{
			GLDrawElementsRenderData* data;
			uint32_t numberOfDraws;
		};
	}
}

#endif