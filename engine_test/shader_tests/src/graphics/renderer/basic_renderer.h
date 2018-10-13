#pragma once

#include <vector>
#include "renderer.h"

class basic_renderer : public renderer
{
private:


	std::vector<glm::mat4> model_matrices;
public:
	auto submit(glm::mat4 const & model_matrix) -> void;
	auto render(void) -> void;
	auto flush(void) -> void;
};