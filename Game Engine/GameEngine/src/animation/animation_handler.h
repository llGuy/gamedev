#pragma once

#include <string>
#include <fstream>

namespace rapidxml { template <typename Ch = char> class xml_node; }

class skeletal_animation_handler
{
private:

public:
	auto load_animation(std::string const & dir) -> void;

private:
	auto get_float_array_str(rapidxml::xml_node<> * source) -> std::string;
};