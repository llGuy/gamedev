#include <vector>
#include <sstream>
#include <algorithm>
#include "../io/io.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include "../utils/types.h"
#include "animation_handler.h"
/* using the xmlrapid library to parse .dae files */
#include <xml_parser/rapidxml.hpp>

#include "joint.h"
#include "key_frame.h"

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


	auto weights_raw = get_joint_weights(weights_source);

	std::vector<glm::vec3> weights;
	std::vector<glm::ivec3> joint_ids;
	load_joint_weights_and_ids(vertex_weights_node, weights, joint_ids, weights_raw);

	std::unordered_map<std::string, joint *> joint_map;
	std::vector<joint *> index_joint_map;
	load_joint_map(joint_map, index_joint_map, joint_names_source);

	auto inverse_transforms = get_inverse_bind_transforms(bind_poses_source);

	/* load joint hierarchy */
	xml_node<> * library_visual_scenes = parsed.first->last_node("COLLADA")->last_node("library_visual_scenes");
	xml_node<> * visual_scene = library_visual_scenes->first_node();

	xml_node<> * armature = visual_scene->first_node()->next_sibling();

	xml_node<> * head = armature->first_node("node");

	load_hierarchy(head, joint_map, nullptr);

	/* FINAL : loading the animation */

	xml_node<> * library_animations = parsed.first->last_node("COLLADA")->last_node("library_animations");
	xml_node<> * first_joint_animation_node = library_animations->first_node();

	std::vector<key_frame> key_frames = get_key_frames(first_joint_animation_node->first_node()->first_node()/* float array */);

	load_key_frame(library_animations->first_node(), key_frames);

	/* TODO : load animation into game! */
}

auto skeletal_animation_handler::load_key_frame(rapidxml::xml_node<char> * animation, std::vector<key_frame> & frames) -> void
{
	using namespace rapidxml;

	for (xml_node<> * a_node = animation; a_node; a_node = a_node->next_sibling())
	{
		/* get to the matrix output */
		xml_node<> * src = a_node->first_node()->next_sibling();

		/* load key frame data */
		/* get name of joint */
		std::string joint_name;
		std::string whole_id = src->first_attribute()->value();
		/* get the joint name */
		std::istringstream stream(whole_id);
		std::string current_word;
		u32 count = 0;
		while (std::getline(stream, current_word, '_'))
			if (count++ == 1)
			{
				joint_name = current_word;
				break;
			}

		/* get float array */
		xml_node<> * float_array = src->first_node();

		std::istringstream stream_floats(float_array->value());
		std::string current_float;
		u32 float_count = 0;
		glm::mat4 current_matrix;
		while (std::getline(stream_floats, current_float, ' '))
		{
			if (float_count % 16 == 0 && float_count != 0)
			{
				/* convert to position and quaternion */
				glm::vec3 position = glm::vec3(current_matrix[3][0], current_matrix[3][1], current_matrix[3][2]);
				glm::quat rotation = glm::quat_cast(current_matrix);

				auto & transform = frames[float_count / 16 - 1][joint_name];
				transform.position = position;
				transform.rotation = rotation;
			}

			current_matrix[(float_count / 4) % 4][float_count % 4] = std::stof(current_float);

			++float_count;
		}
	}
}

auto skeletal_animation_handler::get_key_frames(rapidxml::xml_node<char> * animations_node) -> std::vector<key_frame>
{
	std::vector<key_frame> key_frames;
	
	std::istringstream stream(animations_node->value());
	std::string current_time_stamp;

	while (std::getline(stream, current_time_stamp, ' '))
	{
		key_frames.push_back(key_frame(std::stof(current_time_stamp)));
	}

	return key_frames;
}

auto skeletal_animation_handler::load_hierarchy(rapidxml::xml_node<> * current
	, std::unordered_map<std::string, joint *> & joint_map, joint * parent) -> void
{
	using namespace rapidxml;

	std::string current_name = current->first_attribute()->value();
	joint * current_joint = joint_map[current_name];
	current_joint->get_parent() = parent;
	if (parent) parent->add_child(current_joint);

	glm::mat4 bone_space_transform;

	std::istringstream stream(current->first_node()->value());
	std::string current_float;
	u32 count = 0;

	while (std::getline(stream, current_float, ' '))
		bone_space_transform[(count / 4) % 4][count++ % 4] = std::stof(current_float);

	current_joint->get_local_bind_transform() = bone_space_transform;

	/* load for children */
	auto * first = current->first_node("node");
	if (first)
	{
		for (xml_node<> * child = first; child
			; child = child->next_sibling("node"))
		{
			load_hierarchy(child, joint_map, current_joint);
		}
	}
}

auto skeletal_animation_handler::get_inverse_bind_transforms(rapidxml::xml_node<char> * src)->std::vector<glm::mat4>
{
	using namespace rapidxml;

	xml_node<> * float_array = src->first_node();

	std::vector<glm::mat4> matrices;
	matrices.resize(std::stoi(float_array->last_attribute()->value()) / 16);

	std::istringstream stream(src->first_node()->value());
	std::string current_float;

	u32 count = 0;

	while(std::getline(stream, current_float, ' '))
	{
		f32 f = std::stof(current_float);
		matrices[count / 16][(count / 4) % 4][count % 4] = f;

		++count;
	}

	return matrices;
}

auto skeletal_animation_handler::load_joint_map(std::unordered_map<std::string, joint *> & joint_map
	, std::vector<joint *> & joint_index_map
	, rapidxml::xml_node<char> * src) -> void
{
	using namespace rapidxml;

	std::istringstream name_array(src->first_node()->value());
	std::string current_name;

	u32 index = 0;

	while (std::getline(name_array, current_name, ' '))
	{
		joint_index_map.push_back(new joint(current_name, index++));
		joint_map[current_name] = joint_index_map.back();
	}
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

auto skeletal_animation_handler::get_joint_weights(rapidxml::xml_node<char> * weights_source)->std::vector<f32>
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

auto skeletal_animation_handler::get_animation(std::string const & name) -> animation *
{
	return animations[name];
}