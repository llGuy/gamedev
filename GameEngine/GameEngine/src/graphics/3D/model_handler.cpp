#include <future>
#include "model_handler.h"
#include <glm/gtc/type_ptr.hpp>
#include <xml_parser/rapidxml.hpp>

model_handler::model_handler(void)
	: check_xcp(true)
{
}

auto model_handler::init(skeletal_animation_handler & animations) -> void
{
	components.add_system<joint_ids_buffer_component>(20);
	components.add_system<weights_buffer_component>(20);
	components.add_system<texture_buffer_component>(20);
	components.add_system<vertex_buffer_component>(20);
	components.add_system<tangent_buffer_component>(20);
	components.add_system<normal_buffer_component>(20);
	components.add_system<index_buffer_component>(20);
	components.add_system<color_buffer_component>(20);
	components.add_system<texture_component>(20);

	json_loader.init(*this, animations);
}

auto model_handler::load_from_json(void) -> void
{
	json_loader.load(extract_file("res/saves/models.json"), *this);
}

auto model_handler::init_model(std::string const & model_name) -> model &
{
	models.push_back(model());

	components.ready_object(models.back());

	model_indices[model_name] = models.size() - 1;

	return models.back();
}

auto model_handler::create_shader_handle(model & object) -> shader_handle
{
	shader_handle handle;

	if (object.has_component<normal_buffer_component>()) handle.set(shader_property::vertex_normal);
	else handle.set(shader_property::dynamic_normals, shader_property::linked_to_gsh);

	if (object.has_component<texture_buffer_component>()) handle.set(shader_property::texture_coords);
	else handle.set(shader_property::vertex_color);

	return handle;
}

auto model_handler::get_float_array_str(rapidxml::xml_node<> * source) -> std::string
{
	return source->value();
}

auto model_handler::load_model_from_dae(model & obj, std::string const & file_name
	, std::pair<rapidxml::xml_document<> *, std::string *> parsed) -> std::pair<rapidxml::xml_document<> *, std::string *>
{
	using namespace rapidxml;

	xml_document<> * doc = parsed.first;

	if (!parsed.first)
	{
		parsed.second = new std::string(extract_file(file_name));
		doc = new xml_document<>();
		doc->parse<0>(const_cast<char *>(parsed.second->c_str()));
	}

	xml_node<> * first_node = doc->first_node("COLLADA");

	/* load mesh data of model */
	xml_node<> * mesh         = first_node->first_node("library_geometries")->first_node()->first_node();
	char const * mesh_name = mesh->name();
	xml_node<> * src_vertices = mesh->first_node();
	char const * vertices_name = src_vertices->name();
	xml_node<> * src_normals  = src_vertices->next_sibling();
	xml_node<> * src_uvs      = src_normals->next_sibling();

	std::string vertices_raw = get_float_array_str(src_vertices->first_node());
	std::string normals_raw  = get_float_array_str(src_normals->first_node());
	std::string uvs_raw      = get_float_array_str(src_uvs->first_node());

	u32 vertices_count = std::stoi(src_vertices->first_node()->last_attribute()->value());
	u32 normals_count = std::stoi(src_normals->first_node()->last_attribute()->value());	
	u32 uvs_count = std::stoi(src_uvs->first_node()->last_attribute()->value());

	std::vector<glm::vec3> vertices = extract_vertices_from_line<3>(vertices_raw
		, vertices_count, true);
	std::vector<glm::vec3> normals = extract_vertices_from_line<3>(normals_raw
		, normals_count, false);
	std::vector<glm::vec2> uvs = extract_vertices_from_line<2>(uvs_raw
		, uvs_count, false);

	/* load indices of model */
	xml_node<> * indices_node = mesh->last_node();
	xml_node<> * arr          = indices_node->last_node()->last_node();

	std::vector<glm::vec3> normals_in_order;
	std::vector<glm::vec2> uvs_in_order;

	normals_in_order.resize(vertices.size());
	uvs_in_order.resize(vertices.size());

	std::vector<u32> indices = organize_vertices(arr->value(), normals, uvs, normals_in_order, uvs_in_order
		, std::stoi(indices_node->last_attribute()->value()));

	std::vector<glm::vec3> tangents; /*empty for debugging*/

	create_model(vertices, normals_in_order, uvs_in_order, indices, tangents, obj);

	return std::pair(doc, parsed.second);
}

auto model_handler::load_model_from_ubobj(std::string const & file_name, model & object) -> void
{
	std::ifstream file(file_name, std::ios::binary | std::ios::ate);

	std::size_t file_size_bytes = static_cast<std::size_t>(file.tellg());
	std::vector<char> byte_buffer;
	byte_buffer.resize(file_size_bytes);

	file.clear();
	file.seekg(0);
	file.read(byte_buffer.data(), file_size_bytes);

	struct header
	{
		uint32_t vertices_offset; uint32_t vertices_size;
		uint32_t normals_offset;  uint32_t normals_size;
		uint32_t tcoords_offset;  uint32_t tcoords_size;
		uint32_t indices_offset;  uint32_t indices_size;
	} head = *(header *)(byte_buffer.data());

	auto * p_vertices = (glm::vec3 *)(byte_buffer.data() + head.vertices_offset);
	auto * p_normals = (glm::vec3 *)(byte_buffer.data() + head.normals_offset);
	auto * p_tcoords = (glm::vec2 *)(byte_buffer.data() + head.tcoords_offset);
	auto * p_indices = (uint32_t *)(byte_buffer.data() + head.indices_offset);

	auto f_vertices = std::async(std::launch::async, detail::copy_from_pointer<glm::vec3>, p_vertices, head.vertices_size);
	auto f_normals = std::async(std::launch::async, detail::copy_from_pointer<glm::vec3>, p_normals, head.normals_size);
	auto f_tcoords = std::async(std::launch::async, detail::copy_from_pointer<glm::vec2>, p_tcoords, head.tcoords_size);
	auto f_indices = std::async(std::launch::async, detail::copy_from_pointer<uint32_t>, p_indices, head.indices_size);

	std::vector<glm::vec3> vertices = std::move(f_vertices.get());
	std::vector<glm::vec3> normals = std::move(f_normals.get());
	std::vector<glm::vec2> tcoords = std::move(f_tcoords.get());
	std::vector<uint32_t> indices = std::move(f_indices.get());

	std::vector<glm::vec3> tangents; /*empty for debugging*/

	create_model(vertices, normals, tcoords, indices, tangents, object);
}

auto model_handler::load_model_from_obj(std::string const & file_name, model & object) -> void
{
	std::ifstream file(file_name);

	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texture_coords;
	std::vector<glm::vec3> tangents;
	std::vector<f32> tangent_amounts;

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
			tangents.resize(vertices.size());
			tangent_amounts.resize(vertices.size());
			normals.resize(vertices.size());
			texture_coords.resize(vertices.size());

			break_face_line(words, indices, raw_texture_coords, raw_normals, texture_coords, normals, tangents, tangent_amounts, vertices);

			break;
		}
	}

	while (std::getline(file, line))
	{
		std::vector<std::string> words = split(line, ' ');
		if (words[0] == "f")
		{
			break_face_line(words, indices, raw_texture_coords, raw_normals, texture_coords, normals, tangents, tangent_amounts, vertices);
		}
	}

	for (u32 i = 0; i < tangents.size(); ++i)
	{
		tangents[i] /= tangent_amounts[i];
	}

	create_model(vertices, normals, texture_coords, indices, tangents, object);
}

auto model_handler::create_shader_handle(std::vector<glm::vec2> & texture_coords, std::vector<glm::vec3> & normals) -> shader_handle
{
	shader_handle handle;
	if (normals.size() != 0) handle.set(shader_property::vertex_normal);
	else handle.set(shader_property::dynamic_normals, shader_property::linked_to_gsh);

	if (texture_coords.size() != 0) handle.set(shader_property::texture_coords);
	else handle.set(shader_property::vertex_color);

	return handle;
}

auto model_handler::break_face_line(std::vector<std::string> const & face_line_words,
	std::vector<u32> & indices, std::vector<glm::vec2> const & raw_textures,
	std::vector<glm::vec3> const & raw_normals,
	std::vector<glm::vec2> & textures, std::vector<glm::vec3> & normals,
	std::vector<glm::vec3> & tangents, std::vector<f32> & tangent_amounts,
	std::vector<glm::vec3> & vertices) -> void
{
	std::array<std::vector<std::string>, 3> face_indices;

	std::array<u32, 3> triangle_vertices;

	for (u32 i = 0; i < 3; ++i)
	{
		face_indices[i] = split(face_line_words[i + 1], '/');
		process_vertex(face_indices[i], indices, raw_textures, raw_normals, textures, normals, triangle_vertices[i]);
	}

	glm::vec3 delta_pos1 = vertices[triangle_vertices[1]] - vertices[triangle_vertices[0]];
	glm::vec3 delta_pos2 = vertices[triangle_vertices[2]] - vertices[triangle_vertices[0]];

	glm::vec2 delta_uv1 = textures[triangle_vertices[1]] - textures[triangle_vertices[0]];
	glm::vec2 delta_uv2 = textures[triangle_vertices[2]] - textures[triangle_vertices[0]];

	f32 r = 1.0 / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
	delta_pos1 *= delta_uv2.y;
	delta_pos2 *= delta_uv1.y;
	glm::vec3 tangent = delta_pos1 - delta_pos2;
	tangent *= r;

	tangents[triangle_vertices[0]] = tangent;
	tangents[triangle_vertices[1]] = tangent;
	tangents[triangle_vertices[2]] = tangent;

	++tangent_amounts[triangle_vertices[0]];
	++tangent_amounts[triangle_vertices[1]];
	++tangent_amounts[triangle_vertices[2]];
}
auto model_handler::process_vertex(std::vector<std::string> const & vertex_data,
	std::vector<u32> & indices, std::vector<glm::vec2> const & raw_textures,
	std::vector<glm::vec3> const & raw_normals,
	std::vector<glm::vec2> & textures, std::vector<glm::vec3> & normals
	, u32 & triangle_vertex) -> void
{
	i32 current_vertex = std::stoi(vertex_data[0]) - 1;
	indices.push_back(current_vertex);

	if (raw_textures.size() > 0)
	{
		glm::vec2 current_tex = raw_textures[std::stoi(vertex_data[1]) - 1];
		textures[current_vertex] = current_tex;
	}

	if (raw_normals.size() > 0)
	{
		glm::vec3 current_normal = raw_normals[std::stoi(vertex_data[2]) - 1];
		f32 normal_y = current_normal.y;
		f32 normal_z = current_normal.z;

		normals[current_vertex] = current_normal;
	}

	triangle_vertex = current_vertex;
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
	std::vector<glm::vec2> & texture_coords, std::vector<u32> & indices, std::vector<glm::vec3> & tangents, model & object) -> void
{
	object->vao.create();
	object->vao.bind();


	buffer vertex_buffer = create_buffer(vertices, GL_ARRAY_BUFFER);
	component<vertex_buffer_component, model_data> vcomp{ vertex_buffer_component{vertex_buffer, std::move(vertices)} };
	object.add_component(vcomp);

	attribute v_attrib{ 0, GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
	vertex_buffer.bind(GL_ARRAY_BUFFER);
	object->vao.attach(vertex_buffer, v_attrib);

	if (normals.size() > 0)
	{
		buffer normal_buffer = create_buffer(normals, GL_ARRAY_BUFFER);
		component<normal_buffer_component, model_data> ncomp{ normal_buffer_component{normal_buffer, std::move(normals)} };
		object.add_component(ncomp);

		attribute n_attrib{ 2, GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
		normal_buffer.bind(GL_ARRAY_BUFFER);
		object->vao.attach(normal_buffer, n_attrib);
	}

	if (texture_coords.size() > 0)
	{
		buffer texture_buffer = create_buffer(texture_coords, GL_ARRAY_BUFFER);
		component<texture_buffer_component, model_data> tcomp{ texture_buffer_component{texture_buffer, std::move(texture_coords)} };
		object.add_component(tcomp);

		attribute t_attrib{ 1, GL_FLOAT, 2, GL_FALSE, sizeof glm::vec2, nullptr };
		texture_buffer.bind(GL_ARRAY_BUFFER);
		object->vao.attach(texture_buffer, t_attrib);
	}

	if (tangents.size() > 0)
	{
		buffer tangents_buffer = create_buffer(tangents, GL_ARRAY_BUFFER);
		component<tangent_buffer_component, model_data> tangent_comp{ tangent_buffer_component{tangents_buffer, std::move(tangents)} };
		object.add_component(tangent_comp);

		attribute tangent_attrib{ 3, GL_FLOAT, 3, GL_FALSE, sizeof glm::vec3, nullptr };
		tangents_buffer.bind(GL_ARRAY_BUFFER);
		object->vao.attach(tangents_buffer, tangent_attrib);
	}

	object->count = indices.size();

	buffer index_buffer = create_buffer(indices, GL_ELEMENT_ARRAY_BUFFER);
	component<index_buffer_component, model_data> icomp{ index_buffer_component{ index_buffer, std::move(indices) } };
	object.add_component(icomp);

	unbind_vertex_layouts();
}

auto model_handler::compute_model(model_computation & computation, model & object) -> void
{
	computation.compute(object);
}

auto model_handler::get_model(u32 id)->model &
{
	return models[id];
}

auto model_handler::get_model_id(std::string const & name) -> u32
{
	return model_indices[name];
}

auto model_handler::get_model(std::string const & name) -> model &
{
	return models[model_indices[name]];
}

auto model_handler::load_tangents_and_bitangents(model & dest) -> void
{
	std::vector<glm::vec3> & vertices = dest.get_component<vertex_buffer_component>()->vertices;
	std::vector<glm::vec3> & normals = dest.get_component<normal_buffer_component>()->normals;
	std::vector<glm::vec2> & uvs = dest.get_component<texture_buffer_component>()->uvs;

	for (u32 i = 0; i < vertices.size(); ++i)
	{

	}
}