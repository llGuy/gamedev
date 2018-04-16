#ifndef SHADER_BASE_HEADER
#define SHADER_BASE_HEADER
#include <string>
#include <GL/glew.h>

namespace dawn {

	struct ShaderBase
	{
		std::string dir;
		GLenum type;
	};

}

#endif