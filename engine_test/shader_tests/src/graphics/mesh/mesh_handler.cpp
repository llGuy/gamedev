#include "mesh_handler.h"
#include <glm/gtc/type_ptr.hpp>

mesh_handler::mesh_handler(void)
	: models(30), check_xcp(true)
{
}

auto mesh_handler::init(void) -> void
{
	data_system.add_system<texture_buffer_component>(20);
	data_system.add_system<vertex_buffer_component>(20);
	data_system.add_system<normal_buffer_component>(20);
	data_system.add_system<index_buffer_component>(20);
	data_system.add_system<color_buffer_component>(20);
	data_system.add_system<vertices_component>(20);
	data_system.add_system<texture_component>(20);
}

auto mesh_handler::create_mesh(std::string const & name) -> void
{
	u32 index = models.vec_size();

	mesh_prototype proto;

	models.add(mesh_prototype());

	map_model_locs[name] = index;
}

/*auto mesh_handler::create_render_func(std::string const & name) -> std::unique_ptr<render_func>
{
	auto & mesh = models[get_mesh_index(name)];

	if (mesh.has_component<index_buffer_component>())
	{
		//return std::make_unique<render_indices>(render_indices{name});
	}
	else
	{
		//return std::make_unique<render_func>(render_arrays{name});
	}
	
}*/

auto mesh_handler::get_data(std::string const & instance) -> mesh_data &
{
	return models[get_mesh_index(instance)].get_data();
}

auto mesh_handler::get_mesh_index(std::string const & name) -> u32
{
	if (!check_xcp) return map_model_locs.at(name);

	if (auto model = map_model_locs.find(name); model != map_model_locs.end()) return model->second;
	else throw xcp::model_access_error(name);
}

auto mesh_handler::load_mesh(std::string const & file_name, std::string const & name) -> shader_handle
{
	u32 instance = get_mesh_index(name);

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

	create_mesh(vertices, normals, texture_coords, indices, instance);

	shader_handle handle;
	if (normals.size() != 0) handle.set(shader_property::vertex_normal);
	else
	{
		handle.set(shader_property::dynamic_normals);
		handle.set(shader_property::linked_to_gsh);
	}

	if (texture_coords.size() != 0) handle.set(shader_property::texture_coords);
	else handle.set(shader_property::vertex_color);

	return handle;
}

auto mesh_handler::break_face_line(std::vector<std::string> const & face_line_words,
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
auto mesh_handler::process_vertex(std::vector<std::string> const & vertex_data,
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
auto mesh_handler::split(std::string const & str, char const splitter) -> std::vector<std::string>
{
	std::vector<std::string> words;
	std::string current;
	std::istringstream iss(str);
	while (std::getline(iss, current, splitter)) words.push_back(current);

	return words;
}

auto mesh_handler::create_mesh(std::vector<glm::vec3> & vertices, std::vector<glm::vec3> & normals,
	std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, u32 instance) -> void
{
	buffer vertex_buffer = create_buffer(vertices, GL_ARRAY_BUFFER);
	buffer normal_buffer = create_buffer(normals, GL_ARRAY_BUFFER);
	buffer texture_buffer = create_buffer(texture_coords, GL_ARRAY_BUFFER);

	add_component<vertex_buffer_component>(instance, vertex_buffer);
	add_component<normal_buffer_component>(instance, normal_buffer);
	add_component<texture_buffer_component>(instance, texture_buffer);

	vertex_layout & layout = models[instance].get_data().vao;

	attribute v_attrib{ 0, GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
	attribute n_attrib{ 1, GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
	attribute t_attrib{ 2, GL_FLOAT, 2, GL_FALSE, sizeof glm::vec2, nullptr };

	layout.create();
	layout.bind();

	vertex_buffer.bind(GL_ARRAY_BUFFER);
	layout.attach(vertex_buffer, v_attrib);

	normal_buffer.bind(GL_ARRAY_BUFFER);
	layout.attach(normal_buffer, n_attrib);

	texture_buffer.bind(GL_ARRAY_BUFFER);
	layout.attach(texture_buffer, t_attrib);

	buffer index_buffer = create_buffer(indices, GL_ELEMENT_ARRAY_BUFFER);

	add_component<index_buffer_component>(instance, index_buffer);

	models[instance].get_data().count = indices.size();
	models[instance].get_data().primitive = GL_TRIANGLES;

	unbind_vertex_layouts();
}