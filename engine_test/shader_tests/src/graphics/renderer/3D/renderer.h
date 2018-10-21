#pragma once

#include <glm/glm.hpp>
#include "../pre_render.h"
#include "../render_func.h"
#include "../../../api/api.h"

class mesh_handler;

struct pre_render_ptr
{
	bool on_heap;
	renderer_pre_render * pre_render;


	auto destroy(void) -> void
	{
		if (on_heap) delete pre_render;
	}
};

class renderer
{
protected:
	u32 mesh_id;

	std::unique_ptr<render_func> draw;
	std::vector<pre_render_ptr> pre_renders;
public:
	renderer(void) = default;

	virtual auto render(glsl_program & program, mesh_handler & mh) -> void = 0;
	virtual auto submit(glm::mat4 const & model_matrix) -> void = 0;
	virtual auto flush(void) -> void = 0;
	virtual ~renderer(void) {};

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