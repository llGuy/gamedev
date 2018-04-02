#ifndef MESH_HEADER
#define MESH_HEADER
#include "../buffer/buffer.h"
#include "../buffer/vao.h"
#include <glm/glm.hpp>

namespace dawn {

	// meshes won't go on the stack
	template<uint32_t _Dmx, uint32_t _Dmz, float _Tess>
	class Mesh
	{
	public:
		Mesh(void) = default;
		Mesh(const glm::vec2& t) noexcept
			: m_translation(t), m_precision(1.0f / _Tess)
		{
		}
		~Mesh(void) = default;
		void GenerateData(void)
		{
			GenerateVertices();
			GenerateColors();
			GenerateIndices();
		}

		__forceinline
		const Buffer& GLBuffer(void) const noexcept
		{
			return m_vibuffer;
		}

		__forceinline
		const VAO& GLVAO(void) const noexcept
		{
			return m_vao;
		}
	private:
		__forceinline
		uint32_t VertIndex(const uint32_t& x, const uint32_t& z)
		{
			return x + _Dmx * z;
		}

		void GenerateVertices(void)
		{
			for (uint32_t x = 0; x < _Dmx; ++x)
			{
				for (uint32_t z = 0; z < _Dmz; ++z)
				{
					float worldX = static_cast<float>(x) + m_translation.x * static_cast<float>(_Dmx);
					float worldZ = static_cast<float>(z) + m_translation.y * static_cast<float>(_Dmz);
					uint32_t arrIndex = x + _Dmx * z;

					// calculate height with some generator
					new(m_vertices + arrIndex) glm::vec3(worldX, 0.0f, worldZ);
				}
			}
		}
		void GenerateColors(void)
		{
			for(uint32_t c = 0; c < (_Dmx - 1) * (_Dmz - 1) * 2; ++c)
				new(m_colors + c) glm::vec3(0.8f);
		}
		void GenerateIndices(void)
		{
			uint32_t index = 0;
			for (uint32_t gridsquareX = 0; gridsquareX < _Dmx - 1; ++gridsquareX)
			{
				for (uint32_t gridsquareZ = 0; gridsquareZ < _Dmx - 1; ++gridsquareZ)
				{
					m_indices[index++] = Index(gridsquareX, gridsquareZ);	
					m_indices[index++] = Index(gridsquareX + 1, gridsquareZ);
					m_indices[index++] = Index(gridsquareX, gridsquareZ + 1);

					m_indices[index++] = Index(gridsquareX, gridsquareZ + 1);
					m_indices[index++] = Index(gridsquareX + 1, gridsquareZ);
					m_indices[index++] = Index(gridsquareX + 1, gridsquareZ + 1);
				}
			}
		}
	private:
		// opengl
		void CreateVertexIndexBuffer(void)
		{
			m_vibuffer.Init(sizeof(m_vertices) + sizeof(m_indices), nullptr, GL_DYNAMIC_DRAW);
			m_vibuffer.SubData(0, sizeof(m_vertices), m_vertices);
			m_vibuffer.SubData(sizeof(m_vertices), sizeof(m_indices), m_indices);
		}
		void CreateVertexVAO(void)
		{
			m_vao.Init();

			m_vao.Bind();
			m_vibuffer.Bind(GL_ARRAY_BUFFER);
			m_vao.EnableArray(0);
			m_vao.VertexAttribPtr(0, 3, GL_FLOAT, 3 * sizeof(float), 0);

			m_colorBuffer.Bind(GL_ARRAY_BUFFER);
			m_vao.EnableArray(1);
			m_vao.VertexAttribPtr(1, 3, GL_FLOAT, 3 * sizeof(float), 0);
			m_vao.Divisor(1, 1);
		}
	private:
		const float m_precision;
		glm::vec2 m_translation;
		glm::vec3 m_vertices[_Dmx * _Dmz];
		glm::vec3 m_colors[(_Dmx - 1) * (_Dmz - 1) * 2];
		// number of squares * 6   -> 6 indices per quad
		uint16_t m_indices[(_Dmx - 1) * (_Dmz - 1) * 6];

		Buffer m_vibuffer;			// vertex + index buffer
		Buffer m_colorBuffer;		// buffer for the color
		VAO m_vao;					// vertex vao
	};

	using DawnMesh = Mesh<32, 32, 2.0f>;
}

#endif