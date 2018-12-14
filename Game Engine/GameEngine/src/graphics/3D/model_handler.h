#pragma once

#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iostream>
#include <unordered_map>
#include <glm/gtx/transform.hpp>


#include "model_comp/cube.h"
#include "../../data/vec_dd.h"
#include "../../utils/detail.h"
#include "../shader/shader_handler.h"

#include <glm/glm.hpp>

#include "../../api/api.h"

#include "model_data.h"
#include "component_types.h"

#include "../../data/components.h"

namespace xcp {

	class model_access_error : public gl_xcp
	{
	private:
		std::string model_name;
	public:
		model_access_error(std::string const & name)
			: model_name(name)
			, gl_xcp::gl_xcp("unable to access model that wasn't created")
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



using model = object<model_data>;



namespace rapidxml { 
	
	template <typename Ch> class xml_document; 
	template <typename Ch> class xml_node;

}



/* just stores meshes along with their data */
class model_handler
{
private:
	bool check_xcp;

	/* models don't need any data */
	component_system<model_data> components;

	std::unordered_map<std::string, u32> model_indices;

	std::vector<model> models;
public:
	model_handler(void);

	auto init(void) -> void;

	auto init_model(std::string const & model_name) -> model &;

	auto load_model_from_obj(std::string const & file_name, model & object) -> void;

	auto load_model_from_ubobj(std::string const & file_name, model & object) -> void;

	auto load_model_from_dae(model & obj, std::string const & file_name
		, std::pair<rapidxml::xml_document<char> *, std::string *> parsed = std::pair(nullptr, nullptr)) -> std::pair<rapidxml::xml_document<char> *, std::string *>;

	auto create_shader_handle(model & object) -> shader_handle;

	auto compute_model(model_computation & computation, model & object) -> void;

	auto get_model(std::string const & name) -> model &;

	auto get_model(u32 id) -> model &;

	auto get_model_id(std::string const & name) -> u32;
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

	auto split(std::string const & str, char const splitter)->std::vector<std::string>;

	auto create_model(std::vector<glm::vec3> & vertices, std::vector<glm::vec3> & normals,
		std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, model & object) -> void;

	auto create_shader_handle(std::vector<glm::vec2> & texture_coords, std::vector<glm::vec3> & normals) -> shader_handle;

	auto get_float_array_str(rapidxml::xml_node<char> * source) -> std::string;

	template <u32 Dimension> auto extract_vertices_from_line(std::string const & raw, u32 float_count, bool flip) -> std::vector<glm::vec<Dimension, f32, glm::highp>>
	{
#define CORRECTION glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))

		std::vector<glm::vec<Dimension, f32, glm::highp>> vertices;
		vertices.resize(float_count / Dimension);

		std::istringstream stream(raw);

		u32 counter = 0;
		std::string current_float;

		while (std::getline(stream, current_float, ' ') && counter < float_count)
		{
			u32 at_array = counter / Dimension;
			vertices[at_array][counter % Dimension] = std::stof(current_float);

			if constexpr (Dimension == 3)
			{
				if (flip && (counter + 1) % 3 == 0 && counter != 0)
				{
					vertices[(counter + 1) / 3 - 1] = glm::mat3(CORRECTION) * vertices[(counter + 1) / 3 - 1];
				}
			}
			++counter;
		}

		return vertices;
	}

	auto organize_vertices(std::string const & raw_list
		, std::vector<glm::vec3> const & normals_raw
		, std::vector<glm::vec2> const & uvs_raw
		, std::vector<glm::vec3> & normals_result
		, std::vector<glm::vec2> & uvs_result
		, u32 index_count) -> std::vector<u32>
	{
		std::vector<u32> indices;

		std::istringstream stream(raw_list);

		u32 counter = 0;
		std::string current_int;

		u32 current_vertex;

		while (std::getline(stream, current_int, ' '))
		{
			/* push index */
			if (counter % 4 == 0)
			{
				current_vertex = std::stoi(current_int);
				indices.push_back(current_vertex);
			}
			/* set normal at appropriate location */
			else if (counter % 4 == 1) normals_result[current_vertex] = normals_raw[std::stoi(current_int)];
			/* set uv coordinates at appropriate location */
			else if (counter % 4 == 2) uvs_result[current_vertex] = uvs_raw[std::stoi(current_int)];

			++counter;
		}

		return indices;
	}
public:
	template <typename T> auto create_buffer(std::vector<T> & data, GLenum target) -> buffer
	{
		buffer gpu_buffer;
		gpu_buffer.create();
		gpu_buffer.fill(data.size() * sizeof(T), data.data(), GL_STATIC_DRAW, target);

		return gpu_buffer;
	}
};