#pragma once

#include <vector>
#include "mesh_comp.h"
#include "../../../math/math.h"
#include "../../../utils/types.h"

struct quad2D_mesh_computation : mesh_computation
{
	auto compute(u32 mesh_index, mesh_handler & meshes) -> void override;
	auto create_vertices(void) -> std::vector<f32>;
};