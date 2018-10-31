#pragma once

#include "../graphics/3D/material/material.h"

class skeletal_material : public material
{
private:
	/* joint transforms that will be sent to the shader */
	std::vector<glm::mat4> joint_transforms;
public:
	skeletal_material(model const & renderable, glm::mat4 const & model_matrix);

	auto render(void) -> void override;

	auto get_transforms_array(void) -> std::vector<glm::mat4> &;
};