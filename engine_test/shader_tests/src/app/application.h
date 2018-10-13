#pragma once

#include "../api/api.h"
#include "../win/window.h"
#include "../graphics/graphics.h"

class application
{
private:
	bool is_running;

	window display;

	shader_mapper shaders;
	mesh_handler meshes;

	texture tex;
	vertex_layout vao;
public:
	application(void);

	auto init(void) -> void;
	auto render(void) -> void;
	auto update(void) -> void;
	auto running(void) -> bool;

private:
	auto init_meshes(void) -> void;
	auto init_shaders(void) -> void;
};