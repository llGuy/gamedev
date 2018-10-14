#include "io.h"
#include <stb-master/stb_image.h>

auto extract_file(std::string const & dir) -> std::string
{
	std::ifstream file(dir);

	if (!file.good())
	{
		throw xcp::file_open_error(dir);
	}

	return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

auto extract_png(std::string const & dir) -> image
{
	i32 w, h, num_comp;
	auto * data = stbi_load(dir.c_str(), &w, &h, &num_comp, 4);
	return image{ data, w, h };
}