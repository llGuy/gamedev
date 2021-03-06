#include "texture.h"
#include "vao.h"
#include "buffer.h"
#include "model_handler.h"

#include <glm/gtc/type_ptr.hpp>

model_handler::model_handler(void)
	: models(30), check_xcp(true)
{
}

auto model_handler::init(glm::mat4 & projection_matrix, glm::vec3 & light_pos, glm::mat4 & shadow_bias) -> void
{
	data_system.add_system<batch_rendering_component>(20);
	data_system.add_system<texture_buffer_component>(20);
	data_system.add_system<vertex_buffer_component>(20);
	data_system.add_system<normal_buffer_component>(20);
	data_system.add_system<index_buffer_component>(20);
	data_system.add_system<color_buffer_component>(20);
	data_system.add_system<vertices_component>(20);
	data_system.add_system<texture_component>(20);

	model_shaders.attach(shader(GL_VERTEX_SHADER, vsh_dir));
	model_shaders.attach(shader(GL_GEOMETRY_SHADER, gsh_dir));
	model_shaders.attach(shader(GL_FRAGMENT_SHADER, fsh_dir));

	model_shaders.link("vertex_position", "vertex_normal", "texture_coords", "transform_matrix");

	model_shaders.bind();

	model_shaders.send_uniform_int("diffuse_map", 0);
	model_shaders.send_uniform_int("shadow_map", 1);

	model_shaders.send_uniform_vec3("light_position", glm::value_ptr(light_pos), 1);
}

auto model_handler::prepare(render_pass_data & args) -> void
{
	model_shaders.bind();
	model_shaders.send_uniform_mat4("projection_matrix", glm::value_ptr(args.projection_matrix), 1);
	model_shaders.send_uniform_mat4("view_matrix", glm::value_ptr(args.view_matrix), 1);
	model_shaders.send_uniform_mat4("shadow_bias", glm::value_ptr(args.shadow_bias), 1);
	model_shaders.send_uniform_vec3("light_position", glm::value_ptr(args.light_position), 1);
	model_shaders.send_uniform_vec3("camera_pos", glm::value_ptr(args.camera_pos), 1);
	model_shaders.send_uniform_mat4("model_matrix", glm::value_ptr(detail::identity_matrix), 1);

	args.diffuse.bind(GL_TEXTURE_2D, 0);
	args.depth_map.bind(GL_TEXTURE_2D, 1);
}

auto model_handler::render_model(std::string const & name, glm::mat4 & model_matrix) -> void
{
	u32 instance = get_model_index(name);

	auto & obj = models[instance];
	auto & data = obj.get_data();
	
	auto & renderer = get_component<batch_rendering_component>(name).value;
	renderer.submit(model_matrix);
}

auto model_handler::batch_render_all(void) -> void
{
	model_shaders.bind();

	batch_render_all_raw();
}

auto model_handler::batch_render_all_raw(void) -> void
{
	/* loop through all batch renderers and instance render objects */
	for (u32 i = 0; i < models.vec_size(); ++i)
	{
		auto & obj_data = models[i].get_data();

		if (models[i].has_component<batch_rendering_component>())
		{
			auto & renderer = get_component<batch_rendering_component>(i).value;

			if (renderer.contains_objects())
				renderer.render_indices(obj_data.vao,
					get_buffer<index_buffer_component>(i), GL_TRIANGLES, obj_data.count);
		}
	}
}

auto model_handler::batch_flush_all(void) -> void
{
	auto & batch_system = data_system.get_system<batch_rendering_component>();

	for (u32 i = 0; i < batch_system.size(); ++i)
	{
		batch_system[i].value.value.flush();
	}
}

auto model_handler::render(std::string const & name) -> void
{
	u32 instance = get_model_index(name);

	auto & data = models[get_model_index(name)].get_data();
	data.vao.bind();

	if (models[instance].has_component<index_buffer_component>())
	{
		auto & index_buffer = get_buffer<index_buffer_component>(instance);
		index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);

		glDrawElements(data.primitive, data.count, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(data.primitive, 0, data.count);
	}

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

auto model_handler::create_model(std::string const & name, bool batch) -> void
{
	u32 index = models.vec_size();

	model_prototype proto;

	models.add(model_prototype());

	map_model_locs[name] = index;
}

auto model_handler::get_data(std::string const & instance) -> model_data &
{
	return models[get_model_index(instance)].get_data();
}

auto model_handler::get_model_index(std::string const & name) -> u32
{
	if (!check_xcp) return map_model_locs.at(name);

	if (auto model = map_model_locs.find(name); model != map_model_locs.end()) return model->second;
	else throw xcp::model_access_error(name);
}

auto model_handler::load_model(std::string const & file_name, std::string const & name) -> void
{
	u32 instance = get_model_index(name);

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

	create_model(vertices, normals, texture_coords, indices, instance);
}

auto model_handler::break_face_line(std::vector<std::string> const & face_line_words,
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
auto model_handler::process_vertex(std::vector<std::string> const & vertex_data,
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
auto model_handler::split(std::string const & str, char const splitter) -> std::vector<std::string>
{
	std::vector<std::string> words;
	std::string current;
	std::istringstream iss(str);
	while (std::getline(iss, current, splitter)) words.push_back(current);

	return words;
}

auto model_handler::create_model(std::vector<glm::vec3> & vertices, std::vector<glm::vec3> & normals,
	std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, u32 instance) -> void
{
	buffer vertex_buffer = create_buffer(vertices, GL_ARRAY_BUFFER);
	buffer normal_buffer = create_buffer(normals, GL_ARRAY_BUFFER);
	buffer texture_buffer = create_buffer(texture_coords, GL_ARRAY_BUFFER);

	add_component<vertex_buffer_component>(instance, vertex_buffer);
	add_component<normal_buffer_component>(instance, normal_buffer);
	add_component<texture_buffer_component>(instance, texture_buffer);

	vertex_layout & layout = models[instance].get_data().vao;

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

	add_component<index_buffer_component>(instance, index_buffer);

	models[instance].get_data().count = indices.size();
	models[instance].get_data().primitive = GL_TRIANGLES;

	unbind_vertex_layouts();

	batch_renderer renderer;

	renderer.prepare_model_vao(layout);

	add_component<batch_rendering_component>(instance, renderer);
}