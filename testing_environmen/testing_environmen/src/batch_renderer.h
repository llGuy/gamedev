#pragma once

#include <vector>
#include <optional>
#include <glm/glm.hpp>

#include "vao.h"
#include "buffer.h"

class model_handler;

class batch_renderer
{
private:
	buffer data_buffer;

	u32 max;
	u32 size;
public:
	batch_renderer(void);

	auto prepare_model_vao(vertex_layout & vao) -> void;

	auto submit(glm::mat4 const & transform) -> void;

	auto render_indices(vertex_layout & vao, buffer & indices, GLenum mode, u32 prim_count, u32 index_count) -> void;

	auto flush(void) -> void;
};