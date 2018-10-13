#pragma once

#include <glm/glm.hpp>

class renderer
{
public:
	renderer(void) = default;

	virtual auto submit(glm::mat4 const & model_matrix) -> void = 0;
	virtual auto render(void) -> void = 0;
	virtual auto flush(void) -> void = 0;
};