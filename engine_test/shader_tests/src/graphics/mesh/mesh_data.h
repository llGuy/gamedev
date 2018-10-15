#pragma once

#include <GL/glew.h>

#include "../../api/api.h"
#include "../shader/shader_mapper.h"
#include "../renderer/basic_renderer.h"

struct mesh_data
{
	vertex_layout vao; 
	u32 count; 
	GLenum primitive;

	shader_handle glsl_handle;
	renderer * mesh_renderer;
};