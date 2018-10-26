#pragma once

#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>


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



using model = object<model_data>;



/* just stores meshes along with their data */
class model_handler
{
private:
	bool check_xcp;

	/* models don't need any data */
	component_system<model_data> components;
public:
	model_handler(void);

	auto init(void) -> void;

	auto init_model(void) -> model;

	auto load_model_from_obj(std::string const & file_name, model & object) -> shader_handle;

	auto create_shader_handle(model & object) -> shader_handle;

	auto compute_model(model_computation & computation, model & object) -> void;
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

public:
	template <typename T> auto create_buffer(std::vector<T> & data, GLenum target) -> buffer
	{
		buffer gpu_buffer;
		gpu_buffer.create();
		gpu_buffer.fill(data.size() * sizeof(T), data.data(), GL_STATIC_DRAW, target);

		return gpu_buffer;
	}
};