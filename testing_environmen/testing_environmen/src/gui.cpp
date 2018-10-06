#include "gui.h"
#include "render_func.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

auto gui_handler::create(resource_handler & rh) -> void
{
	quad.create(rh);

	programs[0].attach(shader(GL_VERTEX_SHADER, "shadow/shadow_gui_vsh.shader"));
	programs[0].attach(shader(GL_FRAGMENT_SHADER, "shadow/shadow_gui_fsh.shader"));
	programs[0].link("vertex_position", "texture_coords");

	programs[1].attach(shader(GL_VERTEX_SHADER, "quad_2D_vsh.shader"));
	programs[1].attach(shader(GL_FRAGMENT_SHADER, "quad_2D_fsh.shader"));
	programs[1].link("vertex_position", "texture_coords");
}

auto gui_handler::push(glm::vec2 const & translation, f32 scale) -> void
{
	glm::vec3 trans3D{ translation.x, translation.y, 0 };
	guis.emplace_back(glm::translate(trans3D) * glm::scale(glm::vec3(scale)));
}

auto gui_handler::prepare(gui_slot slot, i32 i) -> void
{
	auto & model_matrix = guis[slot];
	programs[i].bind();
	programs[i].send_uniform_mat4("model_matrix", glm::value_ptr(model_matrix), 1);
}

auto gui_handler::render(model_handler & mh) -> void
{
	mh.render("2D quad");
//	render_model(quad, GL_TRIANGLE_STRIP);
}