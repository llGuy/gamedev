#include <glm/gtc/type_ptr.hpp>
#include "material.h"

auto material_prototype::init(material_light_info const & light_info, light_handler & lights, camera * bound_cam) -> void
{
	this->lights = &lights;
	this->light_info_receive = light_info;
	this->bound_cam = bound_cam;
}

auto material_prototype::toggle_lighting(void) -> void
{
	enabled_lighting ^= 0b0001;
}

auto material_prototype::prepare(camera & scene_camera) -> void
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

	glm::mat4 no_translation = scene_camera.get_view_matrix_without_translation();

	shader->send_uniform_mat4("view_matrix", glm::value_ptr(scene_camera.get_view_matrix()), 1);
	shader->send_uniform_mat4("view_matrix_no_translation", glm::value_ptr(no_translation), 1);
	shader->send_uniform_mat4("projection_matrix", glm::value_ptr(scene_camera.get_projection_matrix()), 1);
	shader->send_uniform_vec3("camera_position", glm::value_ptr(scene_camera.get_position()), 1);
}

auto material_prototype::submit_material(material * mat) -> void
{
	materials.push_back(mat);
}

auto material_prototype::render(camera & scene_camera) -> void
{
	prepare(scene_camera);

	for (auto & mat : materials)
	{
		mat->render(shader);
	}
}

auto material_prototype::flush(void) -> void
{
	materials.clear();
}

auto material_prototype::get_name(void) -> std::string &
{
	return material_type_name;
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

material::material(model const & renderable, glm::mat4 const & transform, u32 material_type_id)
	: renderable(renderable), transform_matrix(transform), material_type_id(material_type_id)
{
}

auto material::render(glsl_program * shader) -> void
{
	shader->bind();
	
	shader->send_uniform_mat4("model_matrix", glm::value_ptr(transform_matrix), 1);

	renderable->vao.bind();
	auto & indices = renderable.get_component<index_buffer_component>();
	indices->value.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, renderable->count, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}