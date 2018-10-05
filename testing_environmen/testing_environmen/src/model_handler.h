#pragma once

#include <array>
#include <string>
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



namespace xcp {

	class model_access_error : public gl_xcp
	{
	private:
		std::string model_name;
	public:
		model_access_error(std::string const & name)
			: model_name(name), gl_xcp::gl_xcp("unable to access model that wasn't created")
		{
		}

		virtual auto what(void) const -> char const * override
		{
			static char buffer[512];

			std::ostringstream str;

			str << gl_xcp::what() << " : " << model_name;

			strncpy_s(buffer, str.str().c_str(), 512);

			return buffer;
		}
	};

}



/* all models have a vertex layout and count */
struct model_data { vertex_layout vao; u32 count; };

using model_prototype = cs::object<model_data>;

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
	bool check_xcp;

	/* models don't need any data */
	cs::component_system<model_data> data_system;
	/* model vaos or just the model handlers */
	vec_dd<model_prototype> models;

	/* map is used to map to a model from external structures */
	/* creates an interface for accessing the model indices to render them etc... */
	std::unordered_map<std::string, u32> map_model_locs;

	/* rendering models color with textures */
	program model_shaders;
public:
	model_handler(void);

	auto init(glm::mat4 & projection_matrix, glm::vec3 & light_pos, glm::mat4 & shadow_bias) -> void;

	auto create_model(std::string const & name) -> void;

	auto get_data(std::string const & name) -> model_data &;

	auto load_model(std::string const & file_name, std::string const & name) -> void;

	/* rendering the models needs a prepare() call followed by all the model draw calls */
	auto prepare(render_pass_data & args) -> void;

	auto render(std::string const & name) -> void;

	auto render_model(std::string const & name, glm::mat4 & model_matrix) -> void;

	template <typename T> auto load_static_model(T const & shape, std::string const & name) -> void
	{
		u32 instance = get_model_index(name);

		shape(*this, models[instance].get_data(), instance);
	}

	template <typename T> auto get_buffer(u32 index) -> buffer &
	{
		auto & obj = models[index];
		return data_system.get_component<T>(obj.get_component_index<T>()).value.value;
	}

	template <typename T> auto get_buffer(std::string const & name) -> buffer &
	{
		u32 instance = get_model_index(name);

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
		std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, u32 index) -> void;
public:
	template <typename T, typename ... C> auto add_component(std::string const & model_name, C ... args) -> void
	{
		add_component(get_model_index(model_name), args...);
	}

	template <typename T, typename ... C> auto add_component(u32 index, C ... args) -> void
	{
		data_system.add_component<T>(models[index], index, T{ args... });
	}

	template <typename T> auto get_component(std::string const & model_name) -> T &
	{
		u32 instance = get_model_index(model_name);

		return data_system.get_component<T>(instance).value;
	}

	template <typename T> auto create_buffer(std::vector<T> & data, GLenum target) -> buffer
	{
		buffer gpu_buffer;
		gpu_buffer.create();
		gpu_buffer.fill(data.size() * sizeof(T), data.data(), GL_STATIC_DRAW, target);

		return gpu_buffer;
	}
private:
	auto get_model_index(std::string const & name) -> u32;
};

#include "static_models.h"