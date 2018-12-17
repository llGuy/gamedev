#pragma once

#include "loader_functor.h"
#include "../component/component.h"
#include "../component/save.h"

struct key_control_functor : scene_loader_functor
{
	auto apply(scene_loader_functor::data_type & data) -> void override
	{
		if (data.iterator->value())
		{
			component<component_behavior_key, game_object_data> key_component(DEFAULT_KEY_BINDINGS, *data.inputs);

			data.obj->add_component(key_component);
		}
	}
};

struct mouse_control_functor : scene_loader_functor
{
	auto apply(scene_loader_functor::data_type & data) -> void override
	{
		if (data.iterator->value())
		{
			component<component_behavior_mouse, game_object_data> mouse_component(*data.inputs, *data.cam);

			data.obj->add_component(mouse_component);
		}
	}
};

struct bind_camera_functor : scene_loader_functor
{
	auto apply(scene_loader_functor::data_type & data) -> void override
	{
		if (data.iterator->value())
		{
			data.dest->bind_camera_to_object(*data.obj);
		}
	}
};

struct model_matrix_functor : scene_loader_functor
{
	auto apply(scene_loader_functor::data_type & data) -> void override
	{
		if (data.iterator->value())
		{
			component<struct component_model_matrix, game_object_data> matrix_component;

			data.obj->add_component(matrix_component);
		}
	}
};

struct render_functor : scene_loader_functor
{
	auto apply(scene_loader_functor::data_type & data) -> void override
	{
		auto render_node_contents = data.iterator->value();

		std::string model_name = "model." + std::string(data.iterator->value()["model"]);
		std::string material_name = "material." + std::string(data.iterator->value()["material"]);

		component<component_render, game_object_data> render_component(data.models->get_model(model_name)
			, data.materials->get_material_id(material_name), *data.materials);

		data.obj->add_component(render_component);
	}
};

struct save_functor : scene_loader_functor
{
	auto apply(scene_loader_functor::data_type & data) -> void override
	{
		if (data.iterator->value())
		{
			component<struct component_save, game_object_data> save_component(data.handle);

			data.obj->add_component(save_component);
		}
	}
};