#include <glm/gtc/type_ptr.hpp>
#include "material.h"

struct material_info_shader_data
{
	glm::vec4 ambient_reflectivity;
	glm::vec4 diffuse_reflectivity;
	glm::vec4 specular_reflectivity;
	f32 shininess_factor;
	f32 reflect_factor;
};

auto material_prototype::init(void) -> void
{
	material_block.create();
	material_block.fill<void>(sizeof(material_info_shader_data), nullptr, GL_DYNAMIC_DRAW, GL_UNIFORM_BUFFER);

	update_uniform_block();
}

auto material_prototype::update_uniform_block(void) -> void
{
	material_block.bind(GL_UNIFORM_BUFFER);

	u8 * ptr = (u8 *)(material_block.map(GL_UNIFORM_BUFFER, GL_READ_ONLY));

	material_info_shader_data data {
	  glm::vec4(light_info_receive.ambient_reflectivity, 1.0f)
	, glm::vec4(light_info_receive.diffuse_reflectivity, 1.0f)
	, glm::vec4(light_info_receive.specular_reflectivity, 1.0f)
	, light_info_receive.shininess_factor
	, light_info_receive.reflect_factor };

	memcpy(ptr, &data, sizeof(material_info_shader_data));

	unmap_buffers(GL_UNIFORM_BUFFER);
}

auto material_prototype::toggle_lighting(void) -> void
{
	enabled_lighting ^= 0b0001;
}

auto material_prototype::prepare(camera & scene_camera) -> void
{
	shader->bind();

	if (enabled_lighting)
	{
		shader->bind_uniform_block(material_block, "material");
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