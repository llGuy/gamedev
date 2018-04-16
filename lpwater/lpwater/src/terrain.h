#pragma once
#include <stdint.h>
#include <glm/glm.hpp>
#include <new>
#include "vao.h"
#include "buffer.h"

class Terrain
{
public:
	Terrain(void)
	{
		m_vertices = new glm::vec3[WIDTH * DEPTH];
		m_indices = new uint16_t[(WIDTH - 1) * (DEPTH - 1) * 6];

		GenerateVertices();
		GenerateIndices();
		CreateBuffer();
		CreateVAO();
	}
	__forceinline
	uint32_t IndexCount(void) const noexcept
	{
		return (WIDTH - 1) * (DEPTH - 1) * 6;
	}
	__forceinline
	void* Offset(void) const noexcept
	{
		return (void*)0;
	}
	VAO* Vao(void) { return &m_vao; }
	Buffer* Buff(void) { return &m_ibuffer; }
private:
	void CreateBuffer(void)
	{
		m_vbuffer.Init<void>(VERTS_SIZE, m_vertices, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		m_ibuffer.Init(INDICES_SIZE, m_indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
	}
	void CreateVAO(void)
	{
		m_vao.Init();

		// positions
		m_vao.Bind();
		m_vbuffer.Bind(GL_ARRAY_BUFFER);
		m_vao.EnableArray(0);
		m_vao.VertexAttribPtr(0, 3, GL_FLOAT, 3 * sizeof(float), 0);
	}
	uint32_t VertIndex(uint32_t x, uint32_t z)
	{
		return x + DEPTH * z;
	}
	void GenerateVertices(void)
	{
		for (float z = 0.0f; z < DEPTH; z += 1.0f)
		{
			for (float x = 0.0f; x < WIDTH; x += 1.0f)
			{
				float worldX = x;
				float worldZ = z;
				uint32_t arrIndex = VertIndex(static_cast<uint32_t>(x), static_cast<uint32_t>(z));

				// calculate height with some generator
				m_vertices[arrIndex] = glm::vec3(worldX, (worldX + worldZ) / 2.0f - (worldX + worldZ), worldZ);
			}
		}
	}
	void GenerateIndices(void)
	{
		uint32_t index = 0;
		for (uint32_t gridsquareZ = 0; gridsquareZ < DEPTH - 1; ++gridsquareZ)
		{
			for (uint32_t gridsquareX = 0; gridsquareX < WIDTH - 1; ++gridsquareX)
			{
				m_indices[index++] = VertIndex(gridsquareX, gridsquareZ);
				m_indices[index++] = VertIndex(gridsquareX, gridsquareZ + 1);
				m_indices[index++] = VertIndex(gridsquareX + 1, gridsquareZ);

				m_indices[index++] = VertIndex(gridsquareX + 1, gridsquareZ);
				m_indices[index++] = VertIndex(gridsquareX + 1, gridsquareZ + 1);
				m_indices[index++] = VertIndex(gridsquareX, gridsquareZ + 1);
			}
		}
	}
private:
	static constexpr int32_t WIDTH = 64;
	static constexpr int32_t DEPTH = 64;
	static constexpr int32_t VERTS_SIZE = sizeof(glm::vec3) * WIDTH * DEPTH;
	static constexpr int32_t INDICES_SIZE = sizeof(uint16_t) * (WIDTH - 1) * (DEPTH - 1) * 6;

	uint16_t* m_indices;
	glm::vec3* m_vertices;

	Buffer m_vbuffer;
	Buffer m_ibuffer;
	VAO m_vao;
};