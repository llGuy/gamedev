#pragma once

#include <array>
#include "../../utils/types.h"

enum class shader_property : u32
{
	vertex_color, vertex_normal, 
	texture_coords, dynamic_normals, 
	linked_to_gsh, sharp_normals, invalid
};

constexpr std::array<char const *, static_cast<u32>(shader_property::invalid)> property_map
{
	"#define USES_COLOR\n", 
	"#define USES_STATIC_NORMALS\n", 
	"#define USES_TEXTURE\n", 
	"#define USES_DYNAMIC_NORMALS\n", 
	"#define LINKED_TO_GSH\n",
	"#define SHARP_NORMALS\n"
};