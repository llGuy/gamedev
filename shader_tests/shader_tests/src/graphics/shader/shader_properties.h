#pragma once

#include <array>
#include "../../utils/types.h"

enum class shader_property : u32
{
	vertex_color, vertex_normal, 
	texture_coords, dynamic_normals, linked_to_gsh, invalid
};

constexpr std::array<char const *, static_cast<u32>(shader_property::invalid)> property_map
{
	"#define USES_COLOR", 
	"#define USES_STATIC_NORMALS", 
	"#define USES_TEXTURE", 
	"#define USES_DYNAMIC_NORMALS", 
	"#define LINKED_TO_GSH"
};