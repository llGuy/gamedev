#pragma once

#include "../../texture/texture_handler.h"
#include "material_handler.h"
#include "material_json_functor.h"

struct shader_material_functor : material_loader_functor
{
	auto apply(material_loader_functor::data_type & data) -> void override
	{
		auto shader_name = "shader." + std::string(data.iterator->value());
		glsl_program * program = data.shaders->operator[](shader_name);

		data.mat->get_shader() = program;
	}
};

struct flush_material_functor : material_loader_functor
{
	auto apply(material_loader_functor::data_type & data) -> void override
	{
		bool flush_each_frame = data.iterator->value();

		data.mat->get_flush_each_frame() = flush_each_frame;
	}
};

struct textures2D_material_functor : material_loader_functor
{
	auto apply(material_loader_functor::data_type & data) -> void override
	{
		std::vector<std::string> textures_2D = data.iterator->value();

		for (auto tex : textures_2D)
		{
			texture * t = data.textures->get_texture("texture." + tex);
			data.mat->set_texture_2D(t);
		}
	}
};

struct textures3D_material_functor : material_loader_functor
{
	auto apply(material_loader_functor::data_type & data) -> void override
	{
		std::vector<std::string> textures_3D = data.iterator->value();

		for (auto tex : textures_3D)
		{
			texture * t = data.textures->get_texture("texture." + tex);
			data.mat->set_texture_3D(t);
		}
	}
};

struct lighting_material_functor : material_loader_functor
{
	auto apply(material_loader_functor::data_type & data) -> void override
	{
		auto lighting_info = data.iterator->value();

		std::vector<f64> ambient = lighting_info.find("ambient").value();
		std::vector<f64> diffuse = lighting_info.find("diffuse").value();
		std::vector<f64> specular = lighting_info.find("specular").value();

		glm::vec3 ambient_vec3 = glm::vec3(ambient[0], ambient[1], ambient[2]);
		glm::vec3 diffuse_vec3 = glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
		glm::vec3 specular_vec3 = glm::vec3(specular[0], specular[1], specular[2]);

		f32 shiny = lighting_info.find("shiny").value();
		f32 reflect = lighting_info.find("reflect").value();

		material_light_info light_info{ ambient_vec3, diffuse_vec3, specular_vec3, shiny, reflect };

		data.mat->get_light_info() = light_info;

		data.mat->update_uniform_block();
	}
};