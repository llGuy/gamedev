#include "io.h"
#include <fstream>
#include "../xcp/exception.h"
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

auto extract_png(std::string const & dir, bool flipvert) -> image
{
	if (flipvert)
	{
		stbi_set_flip_vertically_on_load(1);
	}

	i32 w, h, num_comp;
	auto * data = stbi_load(dir.c_str(), &w, &h, &num_comp, 4);
	stbi_set_flip_vertically_on_load(0);

	return image{ data, w, h };
}