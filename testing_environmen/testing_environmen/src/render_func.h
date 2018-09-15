#pragma once

#include <GL/glew.h>
#include "renderable.h"

namespace {

	auto render_model(renderable & model, GLenum type) -> void
	{
		auto & vao = model.vao();
		vao.bind();
		if (model.indexed())
		{
			model.element_buffer().value()->bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements(type, model.count(), model.index_type(), model.index_ptr());
			unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
		}
		else
		{
			glDrawArrays(type, 0, model.count());
		}
		unbind_vertex_layouts();
	}

	auto clear(i32 flags, f32 r, f32 g, f32 b) -> void
	{
		glClear(flags);
		glClearColor(r, g, b, 1);
	}

}