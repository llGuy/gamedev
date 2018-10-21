#include "basic_renderer.h"
#include "../../../math/math.h"

auto basic_renderer::submit(glm::mat4 const & model_matrix) -> void
{
	model_matrices.push_back(model_matrix);
}

auto basic_renderer::render(glsl_program & program, mesh_handler & mh) -> void
{
	program.bind();

	for (auto & pre_render : pre_renders)
	{
		pre_render.pre_render->prepare(program);
	}

	for (auto & model_matrix : model_matrices)
	{
		program.send_uniform_mat4("model_matrix", glm::value_ptr(model_matrix), 1);

		draw->draw(mh);
	}
}

auto basic_renderer::flush(void) -> void
{
	model_matrices.clear();
}

basic_renderer::~basic_renderer(void)
{
}