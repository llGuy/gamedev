#ifndef _LENS_FLARE_H_
#define _LENS_FLARE_H_

#include <glm/glm.hpp>
#if (defined cpp_no_optional)
#include "optional.hpp"
#else
#include <optional>
#endif
#include "texture.h"
#include "buffer.h"
#include "vao.h"

#include <array>

namespace mulgame {

	class LFlareHandler
	{
	public:
		static constexpr uint32_t NUM_FLARE_TEXTURES = 10;
		LFlareHandler(void);
		std::optional<glm::vec2> SunScreenCoord(const glm::vec3& lightSource, 
			const glm::mat4& view, const glm::mat4& projection);
		void UpdatePositions(const glm::vec3& lightSource,
			const glm::mat4& view, const glm::mat4& projection);

		Texture& Tex(uint32_t index = 0)
		{
			return m_textures[index];
		}
		VAO& VertexArray(void)
		{
			return m_vao;
		}
		Buffer& OGLBuffer(void)
		{
			return m_buffer;
		}
		glm::vec2& Position(uint32_t index = 0)
		{
			return m_flarePositions[index];
		}
		float Scale(uint32_t index = 0)
		{
			return m_flareScales[index];
		}
		bool Visible(void)
		{
			return m_visible;
		}
		float Brightness(void)
		{
			return m_brightness;
		}
	private:
		void CreateBuffer(Buffer& buffer, glm::vec2* vertices, uint32_t size);
		void CreateVAO(VAO& vao, Buffer& buffer);

		void CalculateFlarePositions(const glm::vec2& suntoCenter, const glm::vec2& sunCoords);
	private:
		VAO m_vao;
		Buffer m_buffer;

		std::array<Texture, NUM_FLARE_TEXTURES> m_textures;
		std::array<glm::vec2, NUM_FLARE_TEXTURES> m_flarePositions;
		std::array<float, NUM_FLARE_TEXTURES> m_flareScales;
		glm::vec2 m_center;

		float m_brightness;
		float m_spacing;
		bool m_visible;
	};

}

#endif
