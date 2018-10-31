#pragma once

#include <string>
#include <fstream>
#include "joint.h"
#include "key_frame.h"
#include "animation.h"
#include "../scene/game_object.h"
#include "../graphics/3D/model_handler.h"
#include "../graphics/3D/renderer/renderer3D.h"

namespace rapidxml { 
	
	template <typename Ch> class xml_document; 
	template <typename Ch> class xml_node;

}

class skeletal_animation_handler
{
private:

	renderer3D animation_renderer;
	material_prototype animated_material_type;

	std::unordered_map<std::string, animation *> animations;

public:
	auto init(shader_handler & shaders, light_handler & lights) -> void; 

	/* to separate later */
	auto load_model_animation_data(model & subject, std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void;

	auto load_animation_data(std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void;

	/* requires the parsed version of the same document which was used to load the mesh from the model_handler */
	auto load_animation(std::string const & animation_name, std::pair<rapidxml::xml_document<char> *
		, std::string *> parsed, model & renderable, game_object & entity) -> void;

	auto get_animation(std::string const & name) -> animation *;

	auto get_renderer(void) -> renderer3D &;

	auto get_material_type(void) -> material_prototype &;
private:
	auto get_joint_weights(rapidxml::xml_node<char> * weights_source) -> std::vector<f32>;
	auto get_inverse_bind_transforms(rapidxml::xml_node<char> * src) -> std::vector<glm::mat4>;
	auto get_key_frames(rapidxml::xml_node<char> * animations_node) -> std::vector<key_frame>;

	auto load_joint_weights_and_ids(rapidxml::xml_node<char> * vertex_weights
		, std::vector<glm::vec3> & weights
		, std::vector<glm::ivec3> & joint_ids
		, std::vector<f32> const & weights_raw) -> void;

	auto load_joint_map(std::unordered_map<std::string, joint *> & joint_map
		, std::vector<joint *> & index_joint_map
		, rapidxml::xml_node<char> * src) -> void;

	auto load_hierarchy(rapidxml::xml_node<char> * current
		, std::unordered_map<std::string, joint *> & joint_map
		, joint * parent) -> joint *;

	auto load_key_frame(rapidxml::xml_node<char> * animation, std::vector<key_frame> & frames) -> void;
};