#pragma once

#include "../api/uniform_buffer.h"
#include "../graphics/3D/material/material.h"

class skeletal_material : public material
{
private:
	/* joint transforms that will be sent to the shader */
	std::vector<glm::mat4> joint_transforms;
	
	uniform_buffer uniform_block;
public:
	skeletal_material(model & renderable, glm::mat4 const & model_matrix, u32 material_type_id);

	auto render(glsl_program * shader) -> void override;

	auto get_transforms_array(void) -> std::vector<glm::mat4> &;

	auto get_uniform_block(void) -> uniform_buffer &;
};