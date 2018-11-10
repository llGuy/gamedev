#include <glm/gtc/type_ptr.hpp>
#include "material.h"

auto material_prototype::init(material_light_info const & light_info, light_handler & lights) -> void
{
	this->lights = &lights;
	this->light_info_receive = light_info;
}

auto material_prototype::toggle_lighting(void) -> void
{
	enabled_lighting ^= 0b0001;
}

auto material_prototype::prepare(void) -> void
{
	shader->bind();

	/* TOTO !!! Replace this stuff with uniform buffers */
	if (enabled_lighting)
	{
		shader->send_uniform_vec3("material_info.ambient_reflectivity", glm::value_ptr(light_info_receive.ambient_reflectivity), 1);
		shader->send_uniform_vec3("material_info.diffuse_reflectivity", glm::value_ptr(light_info_receive.diffuse_reflectivity), 1);
		shader->send_uniform_vec3("material_info.specular_reflectivity", glm::value_ptr(light_info_receive.specular_reflectivity), 1);
		shader->send_uniform_float("material_info.shininess_factor", light_info_receive.shininess_factor);
		shader->send_uniform_float("material_info.reflect_factor", light_info_receive.reflect_factor);
		shader->send_uniform_int("lighting", 1);
		lights->prepare_shader(*shader);
	}
	else
	{
		shader->send_uniform_int("lighting", 0);
	}

	for (u32 i = 0; i < textures2D.size(); ++i)
	{
		textures2D[i]->bind(GL_TEXTURE_2D, i); 
	}

	for (u32 i = 0; i < textures_cubemap.size(); ++i)
	{
		textures_cubemap[i]->bind(GL_TEXTURE_CUBE_MAP, i);
	}
}

auto material_prototype::get_shader(void) -> glsl_program * &
{
	return shader;
}

auto material_prototype::get_textures_2D(void)->std::vector<texture *> &
{
	return textures2D;
}

auto material_prototype::get_textures_cubemap(void)->std::vector<texture *> &
{
	return textures_cubemap;
}

material::material(model const & renderable, glm::mat4 const & transform)
	: renderable(renderable), transform_matrix(transform)
{
}

auto material::render(void) -> void
{
	glsl_program * shader = prototype->get_shader();

	shader->bind();
	
	shader->send_uniform_mat4("model_matrix", glm::value_ptr(transform_matrix), 1);

	renderable->vao.bind();
	auto & indices = renderable.get_component<index_buffer_component>();
	indices->value.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, renderable->count, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}