#include <array>
#include <string>
#include "sky_box.h"

auto sky_box::create(resource_handler & rh) -> void
{
	std::array<std::string, 6> files
	{
		"", "", "", "", "", ""
	};

	for (uint32_t i = 0; i < files.size(); ++i)
	{
		auto img = rh.load<image>(files[i]);
		sky_box_texture.fill(0, GL_RGBA, img.w, img.h, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
}