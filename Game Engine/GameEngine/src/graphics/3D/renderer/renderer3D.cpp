#include "renderer3D.h"
#include <glm/gtc/type_ptr.hpp>

auto renderer3D::set_material_prototype(material_prototype const & prototype) -> void
{
	this->material_type = std::move(prototype);
}

auto renderer3D::submit_material(material const & mat) -> void
{
	materials.push_back(mat);

	materials.back().prototype = &material_type;
}

auto renderer3D::set_projection(glm::mat4 & projection) -> void
{
	glsl_program * shader = material_type.get_shader();
	shader->bind();
	shader->send_uniform_mat4("projection_matrix", glm::value_ptr(projection), 1);
}

auto renderer3D::set_view(glm::mat4 & view) -> void
{
	glsl_program * shader = material_type.get_shader();
	shader->bind();
	shader->send_uniform_mat4("view_matrix", glm::value_ptr(view), 1);
}

auto renderer3D::render(void) -> void
{
	material_type.prepare();

	for (auto & mat : materials)
	{
		mat.render();
	}
}

auto renderer3D::flush(void) -> void
{
	materials.clear();
}

auto renderer3D::get_shader(void) -> glsl_program *
{
	return material_type.get_shader();
}