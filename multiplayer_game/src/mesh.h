#ifndef _MESH_HEADER_
#define _MESH_HEADER_

#define BD_HEIGHT_CHECK_TEST false

#include <new>
#include <array>
#include <stdint.h>
#include <glm/glm.hpp>

#include "vao.h"
#include "math.h"
#include "buffer.h"

namespace mulgame {

    template<uint32_t _Dmx, uint32_t _Dmz>
    class Mesh
    {
	static constexpr uint32_t VERTX = _Dmx + 1;
	static constexpr uint32_t VERTZ = _Dmz + 1;
    public:
	Mesh(void)
	    {
	    }
	void Init(void)
	    {
		GenerateVertices();
		GenerateIndices();
		//GenerateColors();

		CreateVIBuffer();
		//CreateColorBuffer();
		CreateVAO();
	    }

	inline
	DElementsData OGLBuffer(void)
	    {
		return { &m_vibuffer, (void*)(m_vertices.size() * sizeof(glm::vec3)), (VERTX - 1) * (VERTZ - 1) * 6 };
	    }
	inline
	VAO& VertexArray(void)
	    {
		return m_vao;
	    }
	// get x and z coord of player to return the height
	float HeightAtPoint(float x, float z)
	    {
#if BC_HEIGHT_CHECK_TEST == true
		float height;
		// get the difference between the tile corner and
		// the actual coordinate
		glm::vec2 positionOnTile(x - floor(x), z - ceil(z));
		// indices of the triangle of the tile that the position
		// is pointing to
		uint16_t triangleIndices[3];

		// calculate the neg-x neg-z coord of tile
		glm::vec2 tilenegxz(floor(x) + static_cast<float>(VERTX) / 2.0f,
				    floor(z) + static_cast<float>(VERTSZ) / 2.0f);

		if (positionOnTile.x <= 1 + positionOnTile.y)
		{
		    triangleIndices[0] = VIndexf(tilenegxz.x, tilenegxz.y);
		    triangleIndices[1] = VIndexf(tilenegxz.x, tilenegxz.y + 1.0f);
		    triangleIndices[2] = VIndexf(tilenegxz.x + 1.0f, tilenegxz.y + 1.0f);

		    glm::vec3 coord1(0.0f, m_vertices[triangleIndices[0]].y, -1.0f);
		    glm::vec3 coord2(0.0f, m_vertices[triangleIndices[1]].y, 0.0f);
		    glm::vec3 coord3(1.0f, m_vertices[triangleIndices[2]].y, 0.0f);
		    height = BarryCentric(coord1, coord2, coord3, glm::vec2(positionOnTile.x, positionOnTile.y));
		}
#endif

		return 0.0f;
	    }
	glm::ivec2 WorldtoMeshSpace(const glm::vec2& pos)
	{
		glm::vec2 fpos = pos + glm::vec2(static_cast<float>(VERTX) / 2.0f, static_cast<float>(VERTZ) / 2.0f);
		return glm::ivec2(static_cast<uint32_t>(fpos.x), static_cast<uint32_t>(fpos.y));
	}
    private:
	uint32_t VIndex(uint32_t x, uint32_t z)
	    {
		return x + VERTX * z;
	    }
	uint32_t VIndexf(float x, float z)
	    {
		return static_cast<uint32_t>(x) + VERTX * static_cast<uint32_t>(z);
	    }

	void GenerateVertices(void)
	    {
		for (uint32_t zi = 0; zi < VERTZ; ++zi)
		{
		    for (uint32_t xi = 0; xi < VERTX; ++xi)
		    {
			float x = static_cast<float>(xi) - static_cast<float>(VERTX) / 2.0f;
			float z = static_cast<float>(zi) - static_cast<float>(VERTZ) / 2.0f;

			uint32_t index = VIndex(xi, zi);

			// calculate height with some generator
			m_vertices[index] = glm::vec3(x, 0.0f, z);
		    }
		}
	    }
	void GenerateIndices(void)
	    {
		uint32_t index = 0;
		for (uint32_t gsz = 0; gsz < VERTZ - 1; ++gsz)
		{
		    for (uint32_t gsx = 0; gsx < VERTX - 1; ++gsx)
		    {
			m_indices[index++] = VIndex(gsx, gsz);
			m_indices[index++] = VIndex(gsx + 1, gsz);
			m_indices[index++] = VIndex(gsx + 1, gsz + 1);

			m_indices[index++] = VIndex(gsx, gsz);
			m_indices[index++] = VIndex(gsx + 1, gsz + 1);
			m_indices[index++] = VIndex(gsx, gsz + 1);
		    }
		}
	    }
	void GenerateColors(void)
	    {
		std::fill(m_colors.begin(), m_colors.end(), glm::vec3(0.3f, 0.3f, 0.3f));
	    }
    private:
	void CreateVIBuffer(void)
	    {
		const uint32_t VERTICES_SIZE_BYTES = sizeof(glm::vec3) * m_vertices.size();
		const uint32_t INDICES_SIZE_BYTES = sizeof(uint16_t) * m_indices.size();
			
		m_vibuffer.Fill<void>(VERTICES_SIZE_BYTES + INDICES_SIZE_BYTES, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
		m_vibuffer.PartialFill(0, VERTICES_SIZE_BYTES, m_vertices.data(), GL_ARRAY_BUFFER);
		m_vibuffer.PartialFill(VERTICES_SIZE_BYTES, INDICES_SIZE_BYTES, m_indices.data(), GL_ELEMENT_ARRAY_BUFFER);
	    }
	void CreateColorBuffer(void)
	    {
		m_colorBuffer.Fill(m_colors.size(), m_colors.data(), GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
	    }
	void CreateVAO(void)
	    {
		m_vao.Bind();
		m_vibuffer.Bind(GL_ARRAY_BUFFER);
		m_vao.Enable(0);
		m_vao.VAPtr(0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);

		// divisor for each triangle
		/*m_colorBuffer.Bind(GL_ARRAY_BUFFER);
		  m_vao.Enable(1);
		  m_vao.VAPtr(1, 3, GL_FLOAT, 3 * sizeof(float), 0);
		  m_vao.Divisor(1, 1);*/
	    }
    private:
	std::array<glm::vec3, VERTX * VERTZ> m_vertices;
	std::array<glm::vec3, (VERTX - 1) * (VERTZ - 1)> m_colors;
	std::array<uint16_t, (VERTX - 1) * (VERTZ - 1) * 6> m_indices;

	// vertex and index buffer
	Buffer m_vibuffer;
	Buffer m_colorBuffer;
	VAO m_vao;
    };

}

#endif
