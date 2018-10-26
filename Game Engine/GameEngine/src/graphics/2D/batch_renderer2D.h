#pragma once

#include <vector>
#include "renderable2D.h"
#include "../../api/api.h"

class batch_renderer2D
{
private:
	u32 count;
	buffer batch_buffer;
	buffer index_buffer;

	vertex_layout vao;
public:
	auto init(void) -> void;

	auto submit(renderable2D & renderable) -> void;

	auto render(void) -> void;

	auto flush(void) -> void;
};