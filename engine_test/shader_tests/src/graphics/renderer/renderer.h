#pragma once

#include <glm/glm.hpp>
#include "render_func.h"
#include "../../api/api.h"

class mesh_handler;

class renderer
{
protected:
	u32 mesh_id;

	std::unique_ptr<render_func> draw;
public:
	renderer(void) = default;

	virtual auto render(glsl_program & program, mesh_handler & mh) -> void = 0;
	virtual auto submit(glm::mat4 const & model_matrix) -> void = 0;
	virtual auto flush(void) -> void = 0;

	auto set_mesh(u32 mesh_id, mesh_handler & mh) -> void;
private:
	inline auto set_rf(std::unique_ptr<render_func> & func) -> void
	{
		draw = std::move(func);
	}
};