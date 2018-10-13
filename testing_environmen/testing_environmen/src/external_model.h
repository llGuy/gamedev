#pragma once

#include "model_handler.h"
#include "types.h"
#include <vector>

class external
{
private:

public:
	auto operator()(model_handler & handler, model_data & data, u32 index) const -> void
	{

	}
private:
	auto load_model(std::string const & file_name) -> void;

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
		std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, u32 index) -> void;
};