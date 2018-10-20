#pragma once

#include <string>
#include "../../api/api.h"
#include "../../math/math.h"
#include "../texture/texture_mapper.h"

/* pre render gets called for each type of mesh / model : can take care of stuff like materials, textures ... */
struct renderer_pre_render
{
	virtual auto prepare(glsl_program & shader) -> void {}
};

struct renderer_pre_render_texture_bind : renderer_pre_render
{
	u32 texture_id;
	GLenum binding_point;
	GLenum binding_number;
	texture_mapper * textures;

	renderer_pre_render_texture_bind(texture_mapper & mapper, GLenum point, GLenum number, std::string const & name)
		: textures(&mapper), binding_point(point), binding_number(number)
	{
		texture_id = textures->get_texture_id(name);
	}

	auto prepare(glsl_program & shader) -> void override
	{
		texture & tex = textures->operator[](texture_id);

		tex.bind(binding_point, binding_number);
	}
};



struct material : renderer_pre_render
{
	glm::vec3 ambient_reflectivity;
	glm::vec3 diffuse_reflectivity;
	glm::vec3 specular_reflectivity;
	f32 shininess_factor;
	f32 reflect_factor;

	material(glm::vec3 const & ambient_ref, glm::vec3 const & diff_ref, glm::vec3 const & spec_ref, f32 shiny, f32 reflect)
		: ambient_reflectivity(ambient_ref), diffuse_reflectivity(diff_ref), specular_reflectivity(spec_ref), shininess_factor(shiny), reflect_factor(reflect)
	{
	}

	auto prepare(glsl_program & program) -> void override
	{
		program.send_uniform_vec3("material_info.ambient_reflectivity", glm::value_ptr(ambient_reflectivity), 1);
		program.send_uniform_vec3("material_info.diffuse_reflectivity", glm::value_ptr(diffuse_reflectivity), 1);
		program.send_uniform_vec3("material_info.specular_reflectivity", glm::value_ptr(specular_reflectivity), 1);
		program.send_uniform_float("material_info.shininess_factor", shininess_factor);
		program.send_uniform_float("material_info.reflect_factor", reflect_factor);
	}
};



struct pre_render_camera_position : renderer_pre_render
{
	glm::vec3 camera_position;

	auto prepare(glsl_program & program) -> void override
	{
		program.send_uniform_vec3("camera_position", glm::value_ptr(camera_position), 1);
	}
};