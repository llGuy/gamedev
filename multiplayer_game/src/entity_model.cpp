#include "entity_model.h"

namespace mulgame {

    EntityModel::EntityModel(void)
    {
	CreateVertices();
	CreateIndices();
	VAOInit();
    }

    void EntityModel::CreateVertices(void)
    {
	// test rectangle
	glm::vec2 verts[3]
	{
	    glm::vec2(-1.0f, -1.0f),
	    glm::vec2(0.0f, 1.0f),
	    glm::vec2(1.0f, -1.0f)
	};
	m_buffer.Fill(sizeof(verts), verts, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
    }

    void EntityModel::CreateIndices(void)
    {
	uint16_t indices[3]
	{
	    0, 1, 2
	};
	m_ibuffer.Fill(sizeof(uint16_t) * 3, indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
    }

    void EntityModel::VAOInit(void)
    {
	m_buffer.Bind(GL_ARRAY_BUFFER);
	
	m_vao.Bind();
	m_vao.Enable(0);
	m_vao.VAPtr(0, 2, GL_FLOAT, 2 * sizeof(float), nullptr);
	m_ibuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
	m_vao.Unbind();
    }

}
