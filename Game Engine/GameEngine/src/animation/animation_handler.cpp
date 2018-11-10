#include <vector>
#include <sstream>
#include <algorithm>
#include "../utils/detail.h"
#include "../io/io.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include "../utils/types.h"
#include "animation_handler.h"
/* using the xmlrapid library to parse .dae files */
#include <xml_parser/rapidxml.hpp>
#include "animation_component.h"
#include "animation_render_component.h"

#include "joint.h"
#include "key_frame.h"

#define CORRECTION glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))

auto skeletal_animation_handler::init(shader_handler & shaders, light_handler & lights, material_handler & materials) -> void
{
	shader_handle animation_shader("shader.animation3D");

	animation_shader.set(shader_property::linked_to_gsh
		, shader_property::sharp_normals
		, shader_property::texture_coords);

	glsl_shader vsh = shaders.create_shader(GL_VERTEX_SHADER, animation_shader, extract_file("src/shaders/animation3D/vsh.shader"));
	glsl_shader gsh = shaders.create_shader(GL_GEOMETRY_SHADER, animation_shader, extract_file("src/shaders/animation3D/gsh.shader"));
	glsl_shader fsh = shaders.create_shader(GL_FRAGMENT_SHADER, animation_shader, extract_file("src/shaders/animation3D/fsh.shader"));

	glsl_program * shader = shaders.combine(animation_shader, vsh, gsh, fsh);

	auto animated_material = materials.add_material("material.animated", material_light_info(), shader, lights);

	animated_material->get_shader() = shader;
	/* rest of the animated material must be initialized outside this class in the animation for customizability */

	animation_renderer.set_material_prototype(animated_material);
}

auto skeletal_animation_handler::load_skeleton(game_object & entity, model & renderable, std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void
{
	using namespace rapidxml;

	xml_node<> * library_controllers = parsed.first->last_node("COLLADA")->last_node("library_controllers")->first_node()->first_node();

	xml_node<> * bind_matrix_node = library_controllers->first_node();
	xml_node<> * joint_names_source = bind_matrix_node->next_sibling();

	std::unordered_map<std::string, joint *> joint_map;
	std::vector<joint *> index_joint_map;
	load_joint_map(joint_map, index_joint_map, joint_names_source);

	/* load joint hierarchy */
	xml_node<> * library_visual_scenes = parsed.first->last_node("COLLADA")->last_node("library_visual_scenes");
	xml_node<> * visual_scene = library_visual_scenes->first_node();
	xml_node<> * armature = visual_scene->first_node()->next_sibling();
	xml_node<> * head = armature->first_node("node");

	joint * root = load_hierarchy(head, joint_map, nullptr);

	root->calculate_inverses();

	component<component_animation3D, game_object_data> entity_animation_component{ this, *root, index_joint_map.size() };
	skeletal_material mat{ renderable, glm::mat4(1.0f) };
	component<component_animation3D_render, game_object_data> entity_animation_render_component{ this, mat, animation_renderer };

	entity.add_component(entity_animation_component);
	entity.add_component(entity_animation_render_component);
}

auto skeletal_animation_handler::load_model_animation_data(model & subject, std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void
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

	subject->vao.bind();
	auto ibo = subject.get_component<index_buffer_component>();
	ibo->value.bind(GL_ELEMENT_ARRAY_BUFFER);

	buffer weights_buffer;
	weights_buffer.create();
	weights_buffer.fill(weights.size() * sizeof(glm::vec3), weights.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
	attribute weights_attribute{ 4, GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
	subject->vao.attach(weights_buffer, weights_attribute);

	buffer joint_ids_buffer;
	joint_ids_buffer.create();
	joint_ids_buffer.fill(joint_ids.size() * sizeof(glm::ivec3), joint_ids.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
	attribute joint_ids_attribute{ 3, GL_INT, 3, GL_FALSE, sizeof glm::ivec3, nullptr };
	subject->vao.attach(joint_ids_buffer, joint_ids_attribute);

	component<weights_buffer_component, model_data> weights_component(weights_buffer_component{ weights_buffer });
	subject.add_component(weights_component);

	component<joint_ids_buffer_component, model_data> joint_ids_component(joint_ids_buffer_component{ joint_ids_buffer });
	subject.add_component(joint_ids_component);
}

auto skeletal_animation_handler::load_animation_data(std::string const & animation_name, game_object & entity
	, std::pair<rapidxml::xml_document<char> *, std::string *> parsed) -> void
{
	using namespace rapidxml;

	xml_node<> * library_controllers = parsed.first->last_node("COLLADA")->last_node("library_controllers")->first_node()->first_node();

	xml_node<> * library_animations = parsed.first->last_node("COLLADA")->last_node("library_animations");
	xml_node<> * first_joint_animation_node = library_animations->first_node();

	std::vector<key_frame> key_frames = get_key_frames(first_joint_animation_node->first_node()->first_node()/* float array */);

	auto & root = entity.get_component<component_animation3D>().root;

	load_key_frame(library_animations->first_node(), key_frames, &root);

	animations[animation_name] = new animation(key_frames, key_frames.back().get_time_stamp());
}

auto skeletal_animation_handler::load_key_frame(rapidxml::xml_node<char> * animation, std::vector<key_frame> & frames, joint * root) -> void
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
		{
			if (current_word == "pose")
			{
				joint_name.pop_back();
				break;
			}
			else if (count++ >= 1)
			{
				joint_name += current_word + '_';
			}
		}

		/* get float array */
		xml_node<> * float_array = src->first_node();

		std::istringstream stream_floats(float_array->value());
		std::string current_float;
		u32 float_count = 0;
		glm::mat4 current_matrix;
		while (std::getline(stream_floats, current_float, ' '))
		{
			current_matrix[(float_count / 4) % 4][float_count % 4] = std::stof(current_float);

			++float_count;

			if (float_count % 16 == 0 && float_count != 0)
			{
				if (joint_name == root->get_name())
				{
					current_matrix = CORRECTION * glm::transpose(current_matrix);
				}
				else current_matrix = glm::transpose(current_matrix);

				/* convert to position and quaternion */
				glm::vec3 position = glm::vec3(current_matrix[3][0], current_matrix[3][1], current_matrix[3][2]);
				glm::quat rotation = glm::quat_cast(current_matrix);

				auto & transform = frames[float_count / 16 - 1][joint_name];
				transform.position = position;
				transform.rotation = rotation;
			}
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
	, std::unordered_map<std::string, joint *> & joint_map, joint * parent) -> joint *
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

	if (!parent) current_joint->get_local_bind_transform() = CORRECTION * glm::transpose(bone_space_transform);
	else current_joint->get_local_bind_transform() = glm::transpose(bone_space_transform);

	/* load for children */
	auto * first = current->first_node("node");

	if (first)
	{
		for (xml_node<> * child = first; child
			; child = child->next_sibling("node"))
			load_hierarchy(child, joint_map, current_joint);
	}

	if (!parent) return current_joint;
	else return nullptr;
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

	std::for_each(matrices.begin(), matrices.end(), [](glm::mat4 & matrix) -> void { matrix = glm::transpose(matrix); });

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

		glm::ivec3 joint_ids(0);
		glm::vec3 weights(0.0f);
		f32 total = 0.0f;
		for (u32 i = 0; i < 3 && i < current_weight_and_joint.size(); ++i)
		{
			joint_ids[i] = current_weight_and_joint[i].first;
			weights[i] = current_weight_and_joint[i].second;
			total += weights[i];
		}

		/* get total and make elemnts of weights add up to 1 */
		for (u32 i = 0; i < 3; ++i)
			weights[i] = std::min(weights[i] / total, 1.0f);
		weights_dest.push_back(weights);
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

auto skeletal_animation_handler::get_renderer(void) -> renderer3D &
{
	return animation_renderer;
}