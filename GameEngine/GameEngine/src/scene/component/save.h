#pragma once

#include <string>
#include "component.h"
#include <nlohmann/json.hpp>

template <> class component<struct component_save, game_object_data> : public component_base
{
private:
	nlohmann::json * handle;

	std::string entity_name;
public:
	component(nlohmann::json * handle)
		: handle(handle)
	{
	}

	auto update(f32 td, vec_dd<game_object> & objects) -> void override
	{
		if (entity_name == "") entity_name = objects[entity_index]->name;
	}

	auto destroy(vec_dd<game_object> & entities) -> void override
	{
		std::string name = entity_name.substr(entity_name.find(".") + 1);

		nlohmann::json::iterator it = handle->find(name);

		nlohmann::json::iterator position_it = it.value().find("position");
		nlohmann::json::iterator direction_it = it.value().find("direction");

		fill_vec3(position_it, entities[entity_index]->position);
		fill_vec3(direction_it, entities[entity_index]->direction);

		std::ofstream file("res/saves/entities.json");
		
		std::string str = handle->dump();
		file.write(str.c_str(), str.length());
		file.close();

		entity_index = -1;
	}
private:
	auto fill_vec3(nlohmann::json::iterator & it, glm::vec3 v) -> void
	{
		it.value()["x"] = v.x;
		it.value()["y"] = v.y;
		it.value()["z"] = v.z;
	}
};