#pragma once

#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "../../data/vec_dd.h"
#include "../../utils/detail.h"
#include "../renderer/render_func.h"
#include "../../data/component_system.h"

#include <glm/glm.hpp>

#include "../../api/api.h"

#include "mesh_data.h"
#include "component_types.h"

#include "../shader/shader_mapper.h"

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

struct mesh_id
{
	char const * name;
	u32 hash;
};

/* to use later for optimizations */
constexpr auto operator"" _mid(char const * str, u32 size) -> mesh_id
{
	return { str, detail::compile_hash(str, size) };
}

using mesh_name = u32;
using mesh_prototype = cs::object<mesh_data>;



/* just stores meshes along with their data */
class mesh_handler
{
private:
	bool check_xcp;

	/* models don't need any data */
	cs::component_system<mesh_data> data_system;
	/* model vaos or just the model handlers */
	vec_dd<mesh_prototype> models;

	/* map is used to map to a model from external structures */
	/* creates an interface for accessing the model indices to render them etc... */
	std::unordered_map<std::string, u32> map_model_locs;
public:
	mesh_handler(void);

	auto init(void) -> void;

	auto create_mesh(std::string const & name) -> void;

	auto get_data(std::string const & name) -> mesh_data &;

	auto load_mesh(std::string const & file_name, std::string const & name) -> shader_handle;

	auto create_render_func(std::string const & name) -> std::unique_ptr<render_func>;

	template <typename T> auto load_static_mesh(T const & shape, std::string const & name) -> void
	{
		u32 instance = get_mesh_index(name);

		shape(*this, models[instance].get_data(), instance);
	}

	template <typename T> auto get_buffer(u32 index) -> buffer &
	{
		auto & obj = models[index];
		return data_system.get_component<T>(obj.get_component_index<T>()).value.value;
	}

	template <typename T> auto get_buffer(std::string const & name) -> buffer &
	{
		u32 instance = get_mesh_index(name);

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

	auto split(std::string const & str, char const splitter)->std::vector<std::string>;

	auto create_mesh(std::vector<glm::vec3> & vertices, std::vector<glm::vec3> & normals,
		std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, u32 index) -> void;
public:
	template <typename T, typename ... C> auto add_component(std::string const & model_name, C ... args) -> void
	{
		add_component(get_mesh_index(model_name), args...);
	}

	template <typename T, typename ... C> auto add_component(u32 index, C ... args) -> void
	{
		data_system.add_component<T>(models[index], index, T{ args... });
	}

	template <typename T> auto get_component(std::string const & model_name) -> T &
	{
		u32 instance = get_mesh_index(model_name);

		return data_system.get_component<T>(instance).value;
	}

	template <typename T> auto create_buffer(std::vector<T> & data, GLenum target) -> buffer
	{
		buffer gpu_buffer;
		gpu_buffer.create();
		gpu_buffer.fill(data.size() * sizeof(T), data.data(), GL_STATIC_DRAW, target);

		return gpu_buffer;
	}

public:
	auto get_mesh_index(std::string const & name)->u32;
};