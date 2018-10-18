#pragma once

#include <vector>
#include "mesh_comp.h"
#include "../../../math/math.h"
#include "../../../utils/types.h"

struct cube_mesh_computation : mesh_computation
{
	auto operator()(u32 mesh_index, mesh_handler & meshes) -> void override;
	auto create_vertices(void) -> std::vector<f32>;
	auto create_indices(void) -> std::vector<u32>;
};