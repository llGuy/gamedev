#pragma once

#include <vector>
#include "mesh_comp.h"
#include "../../../utils/types.h"

struct cube_model_computation : model_computation
{
	auto compute(object<model_data> & obj) -> void override;
	auto create_vertices(void) -> std::vector<f32>;
	auto create_indices(void) -> std::vector<u32>;
};