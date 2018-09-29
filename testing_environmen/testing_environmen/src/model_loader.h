#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <glm/gtc/type_ptr.hpp>

#include "program.h"

struct model
{
	u32 count;

	buffer vertices;
	buffer normals;
	buffer texture_coords;
	buffer indices;
	vertex_layout layout;
};

class model_loader
{
private:
	program shaders;
public:
	auto create(glm::mat4 & projection_matrix, glm::vec3 & light_pos, glm::mat4 & bias) -> void
	{
		shaders.create_shader(GL_VERTEX_SHADER, "model_shaders/vsh.shader");
		shaders.create_shader(GL_GEOMETRY_SHADER, "model_shaders/gsh.shader");
		shaders.create_shader(GL_FRAGMENT_SHADER, "model_shaders/fsh.shader");
		shaders.link_shaders("vertex_position", "vertex_normal", "texture_coords");
		shaders.get_uniform_locations("projection_matrix", "view_matrix", "model_matrix", "light_position", "diffuse", "shadow_map", "camera_pos", "shadow_bias");

		shaders.use();

		shaders.uniform_mat4(glm::value_ptr(projection_matrix), 0);
		shaders.uniform_3f(glm::value_ptr(light_pos), 3);

		shaders.uniform_1i(0, 4);
		shaders.uniform_1i(1, 5);	

		shaders.uniform_mat4(glm::value_ptr(bias), 7);
	}

	auto render(model & to_render, texture & model_tex, glm::mat4 & view, 
		glm::mat4 & projection, glm::mat4 & mmatrix, texture & shadow_map, glm::vec3 & camera_pos, glm::mat4 & bias) -> void
	{
		shaders.use();

		shaders.uniform_mat4(glm::value_ptr(projection), 0);
		shaders.uniform_mat4(glm::value_ptr(view), 1);
		shaders.uniform_mat4(glm::value_ptr(mmatrix), 2);
		shaders.uniform_3f(glm::value_ptr(camera_pos), 6);
		shaders.uniform_mat4(glm::value_ptr(bias), 7);

		model_tex.bind(GL_TEXTURE_2D, 0);
		shadow_map.bind(GL_TEXTURE_2D, 1);

		to_render.layout.bind();
		to_render.indices.bind(GL_ELEMENT_ARRAY_BUFFER);

		glDrawElements(GL_TRIANGLES, to_render.count, GL_UNSIGNED_INT, nullptr);

		unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
		unbind_vertex_layouts();
	}

	auto load_model(std::string const & file_name) -> model
	{
		std::ifstream file(file_name);

		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> texture_coords;

		/* won't be in the  correct order */
		std::vector<glm::vec2> raw_texture_coords;
		std::vector<glm::vec3> raw_normals;
		std::vector<u32> indices;

		std::string line;
		while (std::getline(file, line))
		{
			std::vector<std::string> words = split(line, ' ');

			if (words[0] == "v")
			{
				glm::vec3 vertex;
				for (u32 i = 0; i < 3; ++i) vertex[i] = std::stof(words[i + 1]);
				vertices.push_back(vertex);
			}
			else if (words[0] == "vt")
			{
				glm::vec2 texture_coord;
				for (u32 i = 0; i < 2; ++i) texture_coord[i] = std::stof(words[i + 1]);
				raw_texture_coords.push_back(glm::vec2(texture_coord.x, 1.0f - texture_coord.y));
			}
			else if (words[0] == "vn")
			{
				glm::vec3 normal;
				for (u32 i = 0; i < 3; ++i) normal[i] = std::stof(words[i + 1]);
				raw_normals.push_back(normal);
			}
			else if (words[0] == "f")
			{
				normals.resize(vertices.size());
				texture_coords.resize(vertices.size());

				break_face_line(words, indices, raw_texture_coords, raw_normals, texture_coords, normals);

				break;
			}
		}

		while (std::getline(file, line))
		{
			std::vector<std::string> words = split(line, ' ');
			break_face_line(words, indices, raw_texture_coords, raw_normals, texture_coords, normals);
		}

		return create_model(vertices, normals, texture_coords, indices);
	}
private:
	auto break_face_line(std::vector<std::string> const & face_line_words,
		std::vector<u32> & indices, std::vector<glm::vec2> const & raw_textures,
		std::vector<glm::vec3> const & raw_normals,
		std::vector<glm::vec2> & textures, std::vector<glm::vec3> & normals) -> void
	{
		std::array<std::vector<std::string>, 3> face_indices;
		for (u32 i = 0; i < 3; ++i)
		{
			face_indices[i] = split(face_line_words[i + 1], '/');
			process_vertex(face_indices[i], indices, raw_textures, raw_normals, textures, normals);
		}
	}
	auto process_vertex(std::vector<std::string> const & vertex_data, 
		std::vector<u32> & indices, std::vector<glm::vec2> const & raw_textures,
		std::vector<glm::vec3> const & raw_normals, 
		std::vector<glm::vec2> & textures, std::vector<glm::vec3> & normals) -> void
	{
		i32 current_vertex = std::stoi(vertex_data[0]) - 1;
		indices.push_back(current_vertex);

		glm::vec2 current_tex = raw_textures[std::stoi(vertex_data[1]) - 1];
		textures[current_vertex] = current_tex;

		glm::vec3 current_normal = raw_normals[std::stoi(vertex_data[2]) - 1];
		normals[current_vertex] = current_normal;
	}
	auto split(std::string const & str, char const splitter) -> std::vector<std::string>
	{
		std::vector<std::string> words;
		std::string current;
		std::istringstream iss(str);
		while (std::getline(iss, current, splitter)) words.push_back(current);

		return words;
	}
	 
	template <typename T> auto create_buffer(std::vector<T> & data, GLenum target) -> buffer
	{
		buffer gpu_buffer;
		gpu_buffer.create();
		gpu_buffer.fill(data.size() * sizeof(T), data.data(), GL_STATIC_DRAW, target);

		return gpu_buffer;
	}

	auto create_model(std::vector<glm::vec3> & vertices, std::vector<glm::vec3> & normals,
		std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices) -> model
	{
		buffer vertex_buffer = create_buffer(vertices, GL_ARRAY_BUFFER);
		buffer normal_buffer = create_buffer(normals, GL_ARRAY_BUFFER);
		buffer texture_buffer = create_buffer(texture_coords, GL_ARRAY_BUFFER);

		vertex_layout layout;

		attribute v_attrib{ GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
		attribute n_attrib{ GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
		attribute t_attrib{ GL_FLOAT, 2, GL_FALSE, sizeof glm::vec2, nullptr };

		layout.create();
		layout.bind();

		vertex_buffer.bind(GL_ARRAY_BUFFER);
		layout.attach(vertex_buffer, v_attrib);

		normal_buffer.bind(GL_ARRAY_BUFFER);
		layout.attach(normal_buffer, n_attrib);

		texture_buffer.bind(GL_ARRAY_BUFFER);
		layout.attach(texture_buffer, t_attrib);

		buffer index_buffer = create_buffer(indices, GL_ELEMENT_ARRAY_BUFFER);

		unbind_vertex_layouts();

		return model{ indices.size(), vertex_buffer, normal_buffer, texture_buffer, index_buffer, layout };
	}
};