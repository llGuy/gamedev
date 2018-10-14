#pragma once

#include "../renderer/renderer.h"

class layer
{
private:
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;


public:
	auto refresh(void) -> void;
};