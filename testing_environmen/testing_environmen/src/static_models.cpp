#include "model_handler.h"
#include "static_models.h"

auto shape<cube>::operator()(model_handler & handler, model_data & data, u32 index) const -> void
{
	std::vector<glm::vec3> verts = create_vertices();
	buffer vertex_buffer;
	vertex_buffer.create();
	vertex_buffer.fill(verts.size() * sizeof(glm::vec3), verts.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	std::vector<u32> indices = create_indices();
	buffer index_buffer;
	index_buffer.create();
	index_buffer.fill(indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
	data.count = indices.size();

	data.vao.create();
	data.vao.bind();

	attribute vertex_att{ GL_FLOAT, 3, GL_FALSE, sizeof(glm::vec3), nullptr, false };

	data.vao.attach(vertex_buffer, vertex_att);

	unbind_vertex_layouts();

	handler.add_component<vertex_buffer_component>(index, vertex_buffer);
	handler.add_component<index_buffer_component>(index, index_buffer);
	handler.add_component<vertices_component>(index, std::move(verts));
}