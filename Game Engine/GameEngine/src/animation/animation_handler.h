#pragma once

#include <string>
#include <fstream>

namespace rapidxml { 
	
	template <typename Ch> class xml_document; 
	template <typename Ch> class xml_node;

}

class skeletal_animation_handler
{
private:

public:
	/* requires the parsed version of the same document which was used to load the mesh from the model_handler */
	auto load_animation(std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void;

private:
	auto load_joint_weights(rapidxml::xml_node<char> * weights_source) -> std::vector<f32>;
	auto load_joint_weights_and_ids(rapidxml::xml_node<char> * vertex_weights
		, std::vector<glm::vec3> & weights
		, std::vector<glm::ivec3> & joint_ids
		, std::vector<f32> const & weights_raw) -> void;
};