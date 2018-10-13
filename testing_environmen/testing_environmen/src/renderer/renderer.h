#pragma once

#include "../types.h"
#include "../model_handler.h"
#include <glm/glm.hpp>

class renderer
{
private:
	u32 size;
public:
	renderer(void) = default;
	
	virtual auto submit(glm::mat4 const & transform) -> void = 0;
	virtual auto render_all(model_handler & mh) -> void = 0;
	virtual auto flush(void) -> void = 0;

	inline auto empty(void) -> bool 
	{
		return size == 0;
	}
};