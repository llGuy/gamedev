#include "entity_model.h"

namespace mulgame {

    EntityModel::EntityModel(float radius)
    {
		CreateVertices(radius);
		CreateIndices();
		VAOInit();
    }

    void EntityModel::CreateVertices(float radius)
    {
		// test rectangle
		glm::vec3 verts[]
		{
			// front
			glm::vec3(-radius, -radius,  radius),
			glm::vec3(radius, -radius,  radius),
			glm::vec3(radius,  radius,  radius),
			glm::vec3(-radius,  radius,  radius),
			// back
			glm::vec3(-radius, -radius, -radius),
			glm::vec3(radius, -radius, -radius),
			glm::vec3(radius,  radius, -radius),
			glm::vec3(-radius,  radius, -radius)
		};
		m_buffer.Fill(sizeof(verts), verts, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		m_radius = glm::distance(glm::vec3(0.0f), glm::vec3(radius));
    }

    void EntityModel::CreateIndices(void)
    {
		uint16_t indices[]
		{
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3,
		};
		m_ibuffer.Fill(sizeof(indices), indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
    }

    void EntityModel::VAOInit(void)
    {
		m_buffer.Bind(GL_ARRAY_BUFFER);
		
		m_vao.Bind();
		m_vao.Enable(0);
		m_vao.VAPtr(0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
		m_ibuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
		m_vao.Unbind();
    }

}
