#include "model_handler.h"
#include "model_json_loader.h"
#include "model_loader_apply_funcs.h"
#include "../../animation/animation_handler.h"

auto model_json_loader::init(model_handler & models, skeletal_animation_handler & animations) -> void
{
	map_funcs["path"] = new path_apply_func;
}

auto model_json_loader::load(std::string const & contents, model_handler & dest) -> void
{
	json_handle = nlohmann::json::parse(contents);

	for (nlohmann::json::iterator it = json_handle.begin(); it != json_handle.end(); ++it)
	{
		std::string model_name = "model." + it.key();

		auto & handle = dest.init_model(model_name);

		parse_sub(dest, it, handle);
	}
}

auto model_json_loader::parse_sub(model_handler & models, nlohmann::json::iterator & it, object<model_data> & renderable) -> void
{
	for (nlohmann::json::iterator sub_it = it.value().begin(); sub_it != it.value().end(); ++sub_it)
	{
		map_funcs[sub_it.key()]->apply(models, sub_it, renderable);
	}
}