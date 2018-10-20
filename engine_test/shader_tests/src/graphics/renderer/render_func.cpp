#include <string>

#include "render_func.h"
#include "../mesh/mesh_handler.h"

render_indices::render_indices(u32 model_id)
	: model_id(model_id)
{
}

auto render_indices::draw(mesh_handler & mh) -> void
{
	mesh_data & data = mh[model_id];
	buffer & ibo = mh.get_buffer<index_buffer_component>(model_id);

	data.vao.bind();
	ibo.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

render_arrays::render_arrays(u32 model_id)
	: model_id(model_id)
{
}

auto render_arrays::draw(mesh_handler & mh) -> void
{
	mesh_data & data = mh[model_id];

	data.vao.bind();

	glDrawArrays(data.primitive, 0, data.count);

	unbind_vertex_layouts();
}