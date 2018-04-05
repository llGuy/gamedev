#ifndef RENDER_PARAMS_HEADER
#define RENDER_PARAMS_HEADER

#include "../buffer/vao.h"
#include "../buffer/buffer.h"

namespace dawn {

	struct RenderParametersElements
	{
		VAO* vao;
		Buffer* ibuffer;
		uint32_t indexCount;
		void* offset;
	};

}

#endif