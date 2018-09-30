#pragma once

#include <array>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "component_system.h"
#include "texture.h"

#include <glm/glm.hpp>

#include "vao.h"
#include "buffer.h"
#include "shader_program.h"

/* all models have a vertex layout and count */
struct model_data { vertex_layout vao; u32 count; };

using model_prototype = cs::object<model_data>;
using model_instance = u32;

struct vertex_buffer_component { buffer value; };
struct vertices_component { std::vector<glm::vec3> vertices; };
struct normal_buffer_component { buffer value; };
struct color_buffer_component { buffer value; };
struct texture_buffer_component { buffer value; };
struct index_buffer_component { buffer value; };
struct texture_component { texture value; u32 point; };

struct render_pass_data
{
	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 shadow_bias;
	glm::vec3 light_position;
	glm::vec3 camera_pos;

	texture depth_map;
	texture diffuse;
};

class model_handler
{
private:
	char const * vsh_dir = "model_shaders/vsh.shader";
	char const * gsh_dir = "model_shaders/gsh.shader";
	char const * fsh_dir = "model_shaders/fsh.shader";
private:
	/* models don't need any data */
	cs::component_system<model_data> data_system;
	/* model vaos or just the model handlers */
	vec_dd<model_prototype> models;

	/* rendering models color with textures */
	program model_shaders;
public:
	model_handler(void);

	auto init(glm::mat4 & projection_matrix, glm::vec3 & light_pos, glm::mat4 & shadow_bias) -> void;

	auto create_model(void) -> model_instance;

	auto get_data(model_instance instance) -> model_data &;

	auto load_model(std::string const & file_name, model_instance instance) -> void;

	/* rendering the models needs a prepare() call followed by all the model draw calls */
	auto prepare(render_pass_data & args) -> void;

	auto render(model_instance instance) -> void;
	auto render_model(model_instance instance, glm::mat4 & model_matrix) -> void;

	template <typename T> auto load_static_model(T const & shape, model_instance instance) -> void
	{
		shape(*this, models[instance].get_data(), instance);
	}

	template <typename T> auto get_buffer(model_instance instance) -> buffer &
	{
		auto & obj = models[instance];
		return data_system.get_component<T>(obj.get_component_index<T>()).value.value;
	}
private:
	/* code for loading models from blender */
	auto break_face_line(std::vector<std::string> const & face_line_words,
		std::vector<u32> & indices, std::vector<glm::vec2> const & raw_textures,
		std::vector<glm::vec3> const & raw_normals,
		std::vector<glm::vec2> & textures, std::vector<glm::vec3> & normals) -> void;

	auto process_vertex(std::vector<std::string> const & vertex_data,
		std::vector<u32> & indices, std::vector<glm::vec2> const & raw_textures,
		std::vector<glm::vec3> const & raw_normals,
		std::vector<glm::vec2> & textures, std::vector<glm::vec3> & normals) -> void;

	auto split(std::string const & str, char const splitter) -> std::vector<std::string>;

	auto create_model(std::vector<glm::vec3> & vertices, std::vector<glm::vec3> & normals,
		std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, model_instance instance) -> void;
public:
	template <typename T, typename ... C> auto add_component(model_instance instance, C ... args) -> void
	{
		data_system.add_component<T>(models[instance], instance, args...);
	}

	template <typename T> auto get_component(model_instance instance) -> T &
	{
		return data_system.get_component<T>(instance).value;
	}

	template <typename T> auto create_buffer(std::vector<T> & data, GLenum target) -> buffer
	{
		buffer gpu_buffer;
		gpu_buffer.create();
		gpu_buffer.fill(data.size() * sizeof(T), data.data(), GL_STATIC_DRAW, target);

		return gpu_buffer;
	}
};

#include "static_models.h"