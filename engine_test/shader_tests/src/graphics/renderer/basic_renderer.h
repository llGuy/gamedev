#pragma once

#include <vector>
#include "renderer.h"

class basic_renderer : public renderer
{
private:
	std::vector<glm::mat4> model_matrices;
public:
	auto render(glsl_program & program, mesh_handler & mh) -> void;
	auto submit(glm::mat4 const & model_matrix) -> void;
	auto flush(void) -> void;
};