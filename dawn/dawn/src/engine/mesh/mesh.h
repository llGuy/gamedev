#ifndef MESH_HEADER
#define MESH_HEADER
#include "../buffer/buffer.h"
#include "../buffer/vao.h"
#include "../renderer/render_params.h"
#include "../terrain/chunk/chunk/chunk_space_coord.h"
#include <glm/glm.hpp>
#include <new>

namespace dawn {

	// meshes won't go on the stack
	template<uint32_t _Dmx, uint32_t _Dmz, uint32_t _Tess>
	class Mesh
	{
		// need to add an extra vertex so that
		// there are NO GAPS between the chunks
		static constexpr uint32_t VERTSX = _Dmx + 1;
		static constexpr uint32_t VERTSZ = _Dmz + 1;
	public:
		Mesh(void) = default;
		Mesh(const terrain::chunk::ChunkSpaceCoord& t) noexcept
			: m_translation(static_cast<float>(t.x), static_cast<float>(t.y)), 
			PRECISION(1.0f / static_cast<float>(_Tess)),
			m_renderParams{&m_vao, &m_vibuffer, IndexCount(), Offset()}
		{
		}
		~Mesh(void) = default;
		

		void GenerateData(void)
		{
			GenerateVertices();
			GenerateColors();
			GenerateIndices();
		}
		void CreateOpenGLObjs(void)
		{
			CreateVertexIndexBuffer();
			CreateColorBuffer();
			CreateVertexVAO();
		}


		__forceinline
		const RenderParametersElements& RenderParams(void) const
		{
			return m_renderParams;
		}
	private:
		__forceinline
		constexpr uint32_t IndexCount(void) const noexcept
		{
			return (VERTSX - 1) * (VERTSZ - 1) * 6;
		}
		__forceinline
		constexpr void* Offset(void) const noexcept
		{
			return (void*)(sizeof(m_vertices));
		}
		__forceinline
		uint32_t VertIndex(uint32_t x, uint32_t z)
		{
			return x + VERTSX * z;
		}

		void GenerateVertices(void)
		{
				for (float z = 0.0f; z < VERTSZ; z += PRECISION)
				{
					for (float x = 0.0f; x < VERTSX; x += PRECISION)
					{
						float worldX = x + m_translation.x * static_cast<float>(_Dmx * PRECISION);
						float worldZ = z + m_translation.y * static_cast<float>(_Dmz * PRECISION);
						uint32_t arrIndex = VertIndex(static_cast<uint32_t>(x), static_cast<uint32_t>(z));

						// calculate height with some generator
						new(m_vertices + arrIndex) glm::vec3(worldX, 0.0f, worldZ);
					}
				}
		}
		void GenerateColors(void)
		{
			for (int32_t c = 0; c < (VERTSX - 1) * (VERTSZ - 1) * 2; ++c)
				new(m_colors + c) glm::vec3(0.5f, 0.5f, 1.0f);
		}
		void GenerateIndices(void)
		{
			uint32_t index = 0;
			for (uint32_t gridsquareZ = 0; gridsquareZ < VERTSZ - 1; ++gridsquareZ)
			{
				for (uint32_t gridsquareX = 0; gridsquareX < VERTSX - 1; ++gridsquareX)
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
		// opengl
		void CreateVertexIndexBuffer(void)
		{
			m_vibuffer.Init<void>(sizeof(m_vertices) + sizeof(m_indices), nullptr, GL_DYNAMIC_DRAW);
			m_vibuffer.SubData(0, sizeof(m_vertices), m_vertices);
			m_vibuffer.SubData(sizeof(m_vertices), sizeof(m_indices), m_indices);
		}
		void CreateColorBuffer(void)
		{
			m_colorBuffer.Init(sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);
		}
		void CreateVertexVAO(void)
		{
			m_vao.Init();

			// positions
			m_vao.Bind();
			m_vibuffer.Bind(GL_ARRAY_BUFFER);
			m_vao.EnableArray(0);
			m_vao.VertexAttribPtr(0, 3, GL_FLOAT, 3 * sizeof(float), 0);

			// colors
			m_colorBuffer.Bind(GL_ARRAY_BUFFER);
			m_vao.EnableArray(1);
			m_vao.VertexAttribPtr(1, 3, GL_FLOAT, 3 * sizeof(float), 0);
			m_vao.Divisor(1, 1);
		}
	private:
		const float PRECISION;

		glm::vec2 m_translation;

		// number of squares * 6   -> 6 indices per quad
		uint16_t m_indices[(VERTSX - 1) * (VERTSZ - 1) * 6];
		glm::vec3 m_vertices[VERTSX * VERTSZ];
		glm::vec3 m_colors[(VERTSX - 1) * (VERTSZ - 1) * 2];

		//Buffer m_ibuffer;
		Buffer m_vibuffer;			// vertex + index buffer
		Buffer m_colorBuffer;		// buffer for the color
		VAO m_vao;					// vertex vao

		RenderParametersElements m_renderParams;
	};

	using ChunkMesh = Mesh<32, 32, 1>;
}

#endif