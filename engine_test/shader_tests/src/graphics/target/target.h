#pragma once

#include <string>
#include "layer.h"
#include <unordered_map>
#include "../../api/api.h"
#include "../mesh/mesh_handler.h"
#include "../shader/shader_mapper.h"

class target
{
private:
	framebuffer render_target;

	std::vector<layer> layers;

	std::unordered_map<std::string, u32> index_map;
public:
	auto get_fbo(void) -> framebuffer &;

	auto add_layer(std::string const & name, layer const &) -> u32;

	auto clear(u32 bits) -> void;
	auto clear_color(f32 r, f32 g, f32 b, f32 a) -> void;
	auto render(mesh_handler & meshes, shader_mapper & shaders) -> void;

	auto get_layer_index(std::string const & name) -> u32;
	auto operator[](u32 index) -> layer &;
};