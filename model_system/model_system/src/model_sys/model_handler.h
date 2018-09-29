#pragma once

#include <array>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "component_system.h"

#include "../inc/buffer.h"

struct model_data { vertex_layout vao; };

using model_prototype = object<model_data>;
using model_instance = u32;


struct vertex_buffer_component { buffer value; };
struct normal_buffer_component { buffer value; };
struct color_buffer_component { buffer value; };
struct texture_buffer_component { buffer value; };
struct index_buffer_component { buffer value; };
struct texture_component { buffer value; };


class model_handler
{
private:
	char const * vsh_dir = "";
	char const * gsh_dir = "";
	char const * fsh_dir = "";
private:
	/* models don't need any data */
	component_system<model_data> data_system;
	/* model vaos or just the model handlers */
	vec_dd<model_prototype> models;
public:
	model_handler(void);

	auto init(void) -> void;

	auto create_model(void) -> model_instance;

	auto get_data(model_instance instance) -> model_data &;

	auto load_model(std::string const & file_name, model_instance instance) -> void;
private:
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