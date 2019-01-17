#pragma once

#include <GL/glew.h>

#include "../../api/api.h"

struct model_data
{
	vertex_layout vao;
	u32 count; 

	GLenum mode{ GL_TRIANGLES };
};