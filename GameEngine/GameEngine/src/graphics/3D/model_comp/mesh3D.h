#pragma once

#include <vector>
#include "mesh_comp.h"

struct mesh3D_model_computation : model_computation
{
    u32 x, z;

	mesh3D_model_computation(u32 x, u32 z)
		: x(x), z(z)
	{
	}

    auto compute(object<model_data> & obj) -> void override;

private:
    
	__forceinline auto get_index(u32 x_, u32 z_) -> u32
	{
		return x_ + x * z_;
	}

    auto create_vertices(void) -> std::vector<glm::vec2>;

	auto create_indices(void) -> std::vector<u32>;

};
