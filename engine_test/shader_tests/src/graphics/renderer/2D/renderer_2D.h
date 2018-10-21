#pragma once

#include <array>
#include <glm/glm.hpp>
#include "../../../api/api.h"
#include "../../mesh/mesh_handler.h"

struct vertex2D
{
	glm::vec2 pos;
	glm::vec2 tex_coord;
};

class renderer_2D
{
protected:
	u32 mesh_id;

	std::unique_ptr<render_func> draw;
	std::vector<pre_render_ptr> pre_renders;
public:
	virtual auto submit(std::array<vertex2D, 4> const & vertices) -> void = 0;
	virtual auto render(glsl_program & program, mesh_handler & mh) -> void = 0;
	virtual auto flush(void) -> void = 0;
	virtual ~renderer_2D(void) {};

	auto set_mesh(u32 mesh_id, mesh_handler & mh) -> void;

	inline auto submit_pre_render(renderer_pre_render * const pre_render, bool on_heap = true)
	{
		pre_renders.push_back(pre_render_ptr{ on_heap, pre_render });
	}
private:
	inline auto set_rf(std::unique_ptr<render_func> & func) -> void
	{
		draw = std::move(func);
	}
};