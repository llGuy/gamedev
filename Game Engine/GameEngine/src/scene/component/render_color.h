#pragma once

#include "../game_object.h"
#include "../../graphics/3D/material/material.h"
#include "../../graphics/3D/renderer/renderer3D.h"
#include "../../graphics/3D/material/material_handler.h"

#include "model_matrix.h"

template <> class component <struct component_render, game_object_data> : public component_base
{
private:
	model & renderable;

	material_handler * materials;

	material * mat;
public:
	component(model & renderable, u32 material_type, material_handler & materials)
		: renderable(renderable)
		, mat(new material(renderable, glm::mat4(1.0f), material_type))
		, materials(&materials)
	{
	}

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		auto & object = objects[entity_index];

		auto & model_matrix_component = object.get_component<component_model_matrix>();

		mat->get_transform() = model_matrix_component.get_trs();

		materials->submit(mat);
	}
};

struct render_apply_func : apply_func
{
	model_handler & models;
	material_handler & materials;

	render_apply_func(model_handler & models, material_handler & materials)
		: models(models), materials(materials)
	{
	}

	auto apply(class scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void override
	{
		auto render_node_contents = it.value();

		std::string model_name = "model." + std::string(it.value()["model"]);
		std::string material_name = "material." + std::string(it.value()["material"]);

		component<component_render, game_object_data> render_component(models.get_model(model_name), materials.get_material_id(material_name), materials);

		obj.add_component(render_component);
	}
};