#include "resources.h"
#include <stb-master/stb_image.h>

template <>
auto resource_handler::load <image>(std::string const & dir) -> typename data <image>::type
{
	int32_t w, h, num_comp;
	auto * data = stbi_load((default_directory + dir).c_str(), &w, &h, &num_comp, 4);
	return { data, w, h };
}
