#pragma once

#include <string>
#include <unordered_map>

#include "render_stage.h"
#include "render_stage3D.h"
#include "render_stage2D.h"

class pipeline
{
private:

	std::unordered_map<std::string, u32> render_stage_indices;
	std::vector<render_stage *> render_stages;

public:

	auto execute_stages(void) -> void;

	auto add_render_stage(std::string const & name, render_stage * stage) -> void;

	auto get_render_stage_id(std::string const & name) -> u32;

	auto operator[](std::string const & name) -> render_stage *;

	auto operator[](u32 id) -> render_stage *;

};