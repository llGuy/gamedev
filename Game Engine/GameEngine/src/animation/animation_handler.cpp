#include <vector>
#include "../io/io.h"
#include <glm/glm.hpp>
#include "../utils/types.h"
#include "animation_handler.h"
/* using the xmlrapid library to parse .dae files */
#include <xml_parser/rapidxml.hpp>

auto skeletal_animation_handler::load_animation(std::string const & dir) -> void
{
	std::string dae_contents = extract_file(dir + ".dae");

	using namespace rapidxml;

	/* initializing xml_doc object */
	xml_document<> * dae = new xml_document<>;
	dae->parse<0>(const_cast<char *>(dae_contents.c_str()));

	xml_node<> * first_node = dae->first_node("COLLADA");

	/* load mesh data of model */
	xml_node<> * mesh         = first_node->first_node("library_geometries")->first_node()->first_node();
	xml_node<> * src_vertices = mesh->first_node();
	xml_node<> * src_normals  = src_vertices->next_sibling();
	xml_node<> * src_uvs      = src_normals->next_sibling();

	std::string vertices_str = get_float_array_str(src_vertices);
}

auto skeletal_animation_handler::get_float_array_str(rapidxml::xml_node<> * source)->std::string
{
	using namespace rapidxml;

	xml_node<> * float_array = source->first_node();

	return "";
}