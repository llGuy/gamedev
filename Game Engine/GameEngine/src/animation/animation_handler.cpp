#include <vector>
#include <sstream>
#include <algorithm>
#include "../io/io.h"
#include <glm/glm.hpp>
#include "../utils/types.h"
#include "animation_handler.h"
/* using the xmlrapid library to parse .dae files */
#include <xml_parser/rapidxml.hpp>

auto skeletal_animation_handler::load_animation(std::pair<rapidxml::xml_document<> *, std::string *> parsed) -> void
{
	using namespace rapidxml;

	xml_node<> * library_controllers = parsed.first->last_node("COLLADA")->last_node("library_controllers")->first_node()->first_node();

	xml_node<> * bind_matrix_node = library_controllers->first_node();
	xml_node<> * joint_names_source = bind_matrix_node->next_sibling();
	xml_node<> * bind_poses_source = joint_names_source->next_sibling();
	xml_node<> * weights_source = bind_poses_source->next_sibling();
	xml_node<> * joints_node = weights_source->next_sibling();
	xml_node<> * vertex_weights_node = joints_node->next_sibling();

	auto weights_raw = load_joint_weights(weights_source);

	std::vector<glm::vec3> weights;
	std::vector<glm::ivec3> joint_ids;

	load_joint_weights_and_ids(vertex_weights_node, weights, joint_ids, weights_raw);
}

auto skeletal_animation_handler::load_joint_weights_and_ids(rapidxml::xml_node<char> * vertex_weights
	, std::vector<glm::vec3> & weights_dest
	, std::vector<glm::ivec3> & joint_ids_dest
	, std::vector<f32> const & weights_raw) -> void
{
	std::istringstream vcount(vertex_weights->last_node("vcount")->value());
	std::istringstream weights_and_ids(vertex_weights->last_node("v")->value());
	std::string current;

	std::vector<u32> vcount_array;
	while (std::getline(vcount, current, ' '))
		vcount_array.push_back(std::stoi(current));

	std::vector<u32> weights_and_ids_array;
	while (std::getline(weights_and_ids, current, ' '))
		weights_and_ids_array.push_back(std::stoi(current));

	for (u32 vcount_index = 0, weights_ids_index = 0; vcount_index < vcount_array.size(); ++vcount_index)
	{
		u32 weight_count = vcount_array[vcount_index];

		std::vector<std::pair<u32, f32>> current_weight_and_joint;

		for (u32 i = weights_ids_index; weights_ids_index < i + weight_count * 2; weights_ids_index += 2)
		{
			u32 joint_id = weights_and_ids_array[weights_ids_index];
			u32 weight_id = weights_and_ids_array[weights_ids_index + 1];

			current_weight_and_joint.push_back(std::pair(joint_id, weights_raw[weight_id]));
		}

		std::sort(current_weight_and_joint.begin()
			, current_weight_and_joint.end()
			, [](std::pair<u32, f32> & lhs, std::pair<u32, f32> & rhs) -> bool { return lhs.second > rhs.second; });

		/* fill glm::ivec3 and glm::vec3 */
		glm::ivec3 joint_ids(-1);
		glm::vec3 weights(0.0f);
		for (u32 i = 0; i < 3 && i < current_weight_and_joint.size(); ++i)
		{
			joint_ids[i] = current_weight_and_joint[i].first;
			weights[i] = current_weight_and_joint[i].second;
		}

		weights_dest.push_back(glm::normalize(weights));
		joint_ids_dest.push_back(joint_ids);
	}
}

auto skeletal_animation_handler::load_joint_weights(rapidxml::xml_node<char> * weights_source)->std::vector<f32>
{
	using namespace rapidxml;

	xml_node<> * float_array = weights_source->first_node("float_array");
	std::istringstream stream(float_array->value());

	u32 count = std::stoi(float_array->last_attribute()->value());

	std::vector<f32> weights;
	weights.reserve(count);

	std::string current_float;

	while (std::getline(stream, current_float, ' '))
	{
		weights.push_back(std::stof(current_float));
	}

	return weights;
}