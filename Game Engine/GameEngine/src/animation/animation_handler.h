#pragma once

#include <string>
#include <fstream>
#include "joint.h"
#include "key_frame.h"
#include "animation.h"
#include "../scene/game_object.h"
#include "../graphics/3D/model_handler.h"
#include "../graphics/3D/renderer/renderer3D.h"
#include "../graphics/3D/material/material_handler.h"

namespace rapidxml { 
	
	template <typename Ch> class xml_document; 
	template <typename Ch> class xml_node;

}

class skeletal_animation_handler
{
private:

	renderer3D animation_renderer;

	std::unordered_map<std::string, animation *> animations;

public:
	auto init(shader_handler & shaders, light_handler & lights, material_handler & materials) -> void; 

	/* loads the bones to a animation component and animation render component */
	auto load_skeleton(game_object & entity, model & renderable, std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void;

	/* loads the weights and joint ids for each vertex */
	auto load_model_animation_data(model & subject, std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void;

	/* requires the skeleton to be loaded and loads the animation data itself */
	auto load_animation_data(std::string const & animation_name, game_object & entity
		, std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void;

	auto get_animation(std::string const & name) -> animation *;

	auto get_renderer(void) -> renderer3D &;
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

	auto load_key_frame(rapidxml::xml_node<char> * animation, std::vector<key_frame> & frames, joint * root) -> void;
};