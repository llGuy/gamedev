#include "quad_3D.h"

quad_3D::quad_3D(glm::vec3 const & p1, glm::vec3 const & p2,
	glm::vec3 const & p3, glm::vec3 const & p4)
	: verts { p1, p2, p3, p4 }
{
}

auto quad_3D::create(resource_handler & rh) -> void 
{
	vertex_buffer.create();
	vertex_buffer.fill(verts.size() * sizeof(glm::vec3), verts.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	layout.create();
	layout.bind();
	layout.attach(vertex_buffer, attribute{ GL_FLOAT, 3, GL_FALSE, 3 * sizeof(float), nullptr, false });
}

auto quad_3D::destroy(void) -> void
{

}