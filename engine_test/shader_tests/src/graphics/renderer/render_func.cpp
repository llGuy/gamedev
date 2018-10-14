#include <string>

#include "render_func.h"
#include "../mesh/mesh_handler.h"

render_indices::render_indices(std::string const & name)
	: name(name)
{
}

auto render_indices::draw(mesh_handler & mh) -> void
{
	mesh_data & data = mh.get_data(name);
	buffer & ibo = mh.get_buffer<index_buffer_component>(name);

	data.vao.bind();
	ibo.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

render_arrays::render_arrays(std::string const & name)
	: name(name)
{
}

auto render_arrays::draw(mesh_handler & mh) -> void
{
	mesh_data & data = mh.get_data(name);

	data.vao.bind();

	glDrawArrays(GL_TRIANGLES, 0, data.count);

	unbind_vertex_layouts();
}