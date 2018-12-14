#include "scene.h"
#include "loader.h"
#include "component/save.h"
#include "component/component.h"

auto scene_loader::init(scene & dest, input_handler & inputs, model_handler & models, material_handler & materials) -> void
{
	map_funcs["position"] = new position_apply_func;
	map_funcs["direction"] = new direction_apply_func;
	map_funcs["scale"] = new scale_apply_func;

	map_funcs["render"] = new render_apply_func{ models, materials };
	map_funcs["model_matrix"] = new model_matrix_apply_func;
	map_funcs["key_control"] = new key_control_apply_func{ inputs };
	map_funcs["mouse_control"] = new mouse_control_apply_func{ inputs, dest.get_scene_camera() };
	map_funcs["bind_camera"] = new bind_camera_apply_func{ dest.get_scene_camera() };
	map_funcs["save"] = new save_apply_func{ &json_handle };
}

auto scene_loader::load(std::string const & contents, scene & dest) -> void
{
	json_handle = nlohmann::json::parse(contents);

	for (nlohmann::json::iterator it = json_handle.begin(); it != json_handle.end(); ++it)
	{
		/* init entity name */
		game_object_data data;
		data.name = "game_object." + it.key();

		ENG_INFO("Loaded entity : ", data.name);

		dest.init_game_object(data);

		parse_sub(dest, it, dest.get_game_object(data.name));
	}
}

auto scene_loader::parse_sub(scene & dest, nlohmann::json::iterator & it, game_object & obj) -> void
{
	for (nlohmann::json::iterator sub_it = it.value().begin(); sub_it != it.value().end(); ++sub_it)
	{
		map_funcs[sub_it.key()]->apply(dest, sub_it, obj);
	}
}