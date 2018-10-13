#include "basic_renderer.h"

auto basic_renderer::submit(glm::mat4 const & model_matrix) -> void
{
	model_matrices.push_back(model_matrix);
}

auto basic_renderer::render(void) -> void
{

}

auto basic_renderer::flush(void) -> void
{
	model_matrices.clear();
}