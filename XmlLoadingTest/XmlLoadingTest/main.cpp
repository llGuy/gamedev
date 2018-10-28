#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include "rapidxml.hpp"

auto main(void) -> int
{
	using namespace rapidxml;

	std::ifstream file("model.dae");

	if (!file.good())
	{
		std::cerr << "unable to load file" << std::endl;
		return -1;
	}

	std::string dae_file = std::string(std::istreambuf_iterator<char>(file)
		, std::istreambuf_iterator<char>());

	xml_document<> * doc = new xml_document<>;
	doc->parse<0>(const_cast<char *>(dae_file.c_str()));

	xml_node<> * node = doc->first_node("COLLADA");

	for (xml_attribute<> * attr = node->first_attribute()
		; attr
		; attr = attr->next_attribute())
	{
		std::cout << attr->name() << "=" << attr->value() << std::endl;
	}



	/* data on the vertices, normals etc... */
	xml_node<> * geometry_library = node->first_node("library_geometries");

	xml_node<> * geometries_node = geometry_library->first_node();
	xml_node<> * mesh_node = geometries_node->first_node();
	xml_node<> * source_id = mesh_node->first_node();
	xml_node<> * vertices = source_id->first_node();

	std::cout << vertices->value() << std::endl;

	xml_node<> * source_normals = source_id->next_sibling();
	xml_node<> * normals = source_normals->first_node();

	std::cout << normals->value() << std::endl;

//	for (xml_attribute<> * attr = )

	std::cin.get();
}