#pragma once

#include "../../../utils/types.h"

class mesh_handler;

/* computes the model without loading from e.g. blender */
struct mesh_computation
{
	virtual auto compute(u32 mesh_index, mesh_handler & meshes) -> void {}
};