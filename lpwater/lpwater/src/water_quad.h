#pragma once
#include <glm/glm.hpp>
#include <stdint.h>
#include <new>

#include "buffer.h"
#include "vao.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class WaterQuad
{
public:
	WaterQuad(void)
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
		float height = -30.0f;
		new(m_vertices + 0) Vertex{ glm::vec3(0.0f, height, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
		new(m_vertices + 1) Vertex{ glm::vec3(WIDTH, height, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
		new(m_vertices + 2) Vertex{ glm::vec3(0.0f, height, DEPTH), glm::vec3(0.0f, 0.0f, 1.0f) };
		new(m_vertices + 3) Vertex{ glm::vec3(WIDTH, height, DEPTH), glm::vec3(0.0f, 0.0f, 1.0f) };
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
		m_vao.Bind();
		
		m_vao.EnableArray(0);
		m_vao.EnableArray(1);
		
		m_vao.VertexAttribPtr(0, 3, GL_FLOAT, 6 * sizeof(float), nullptr);
		m_vao.VertexAttribPtr(1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		m_vao.UnBind();
	}
private:
	static constexpr float WIDTH = 64.0f;
	static constexpr float DEPTH = 64.0f; 

	Vertex m_vertices[4];
	uint16_t m_indices[6];

	Buffer m_vibuffer;
	VAO m_vao;
};