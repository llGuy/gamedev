#pragma once

#include <glm/glm.hpp>
#include <new>
#include "buffer.h"
#include "vao.h"

struct TexturedVertexGUI
{
	glm::vec2 position;
	glm::vec2 textureCoord;
};

class GUI
{
public:
	GUI(void)
	{
		GenerateVertices();
		GenerateIndices();
		CreateBuffer();
		CreateVAO();
	}
	VAO* Vao(void)
	{
		return &m_vao;
	}
	Buffer* Buff(void)
	{
		return &m_vibuffer;
	}
	void* Offset(void)
	{
		return (void*)(sizeof(m_vertices));
	}
	uint32_t Count(void)
	{
		return 6;
	}
private:
	void GenerateVertices(void)
	{
		new(m_vertices + 0) TexturedVertexGUI{ glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f)};
		new(m_vertices + 1) TexturedVertexGUI{ glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f) };
		new(m_vertices + 2) TexturedVertexGUI{ glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f) };
		new(m_vertices + 3) TexturedVertexGUI{ glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	}
	void GenerateIndices(void)
	{
		int32_t index = 0;
		m_indices[index++] = 0;
		m_indices[index++] = 1;
		m_indices[index++] = 2;

		m_indices[index++] = 2;
		m_indices[index++] = 1;
		m_indices[index++] = 3;
	}
	void CreateBuffer(void)
	{
		m_vibuffer.Init<void>(sizeof(m_vertices) + sizeof(m_indices), nullptr, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		m_vibuffer.SubData(0, sizeof(m_vertices), m_vertices);
		m_vibuffer.SubData(sizeof(m_vertices), sizeof(m_indices), m_indices);
	}
	void CreateVAO(void)
	{
		m_vao.Init();
		m_vao.EnableArray(0);
		m_vao.EnableArray(1);
		m_vao.VertexAttribPtr(0, 2, GL_FLOAT, 4 * sizeof(float), 0);
		m_vao.VertexAttribPtr(1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		m_vao.UnBind();
	}
private:
	TexturedVertexGUI m_vertices[4];
	uint16_t m_indices[6];

	Buffer m_vibuffer;
	VAO m_vao;
};