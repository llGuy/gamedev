#include "gui.h"
#include "render_func.h"
#include <glm/gtx/transform.hpp>

auto gui_handler::create(resource_handler & rh) -> void
{
	quad.create(rh);

	programs[0].create_shader(GL_VERTEX_SHADER, "shadow/shadow_gui_vsh.shader");
	programs[0].create_shader(GL_FRAGMENT_SHADER, "shadow/shadow_gui_fsh.shader");
	programs[0].link_shaders("vertex_position", "texture_coords");
	programs[0].get_uniform_locations("model_matrix");

	programs[1].create_shader(GL_VERTEX_SHADER, "quad_2D_vsh.shader");
	programs[1].create_shader(GL_FRAGMENT_SHADER, "quad_2D_fsh.shader");
	programs[1].link_shaders("vertex_position", "texture_coords");
	programs[1].get_uniform_locations("model_matrix");
}

auto gui_handler::push(glm::vec2 const & translation, f32 scale) -> void
{
	glm::vec3 trans3D{ translation.x, translation.y, 0 };
	guis.emplace_back(glm::translate(trans3D) * glm::scale(glm::vec3(scale)));
}

auto gui_handler::prepare(gui_slot slot, i32 i) -> void
{
	auto & model_matrix = guis[slot];
	programs[i].use();
	programs[i].uniform_mat4(&model_matrix[0][0], 0);
}

auto gui_handler::render(void) -> void
{
	render_model(quad, GL_TRIANGLE_STRIP);
}