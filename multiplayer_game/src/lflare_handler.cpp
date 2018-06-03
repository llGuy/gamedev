#include "lflare_handler.h"

#include <new>

namespace mulgame {

	LFlareHandler::LFlareHandler(void)
		: m_center(0.5f), m_spacing(0.4f),
		m_flareScales{ 0.5f, 0.23f, 0.1f, 0.05f, 0.06f, 0.07f, 0.2f, 0.07f, 0.3f, 0.4f,  }
	{
		new(&m_textures[0]) Texture{ "res/lensflare/sun.png" };
		std::string tex = "res/lensflare/tex";
		for(uint32_t i = 1; i < NUM_FLARE_TEXTURES; ++i) new(&m_textures[i]) Texture{ tex + std::to_string(i) + ".png" };
		std::fill(m_flarePositions.begin(), m_flarePositions.end(), glm::vec2(0.0f));

		glm::vec2 vertices[]
		{
			glm::vec2(-1.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			
			glm::vec2(-1.0f, -1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),

			glm::vec2(1.0f, -1.0f),
			glm::vec2(1.0f, 1.0f),
		};

		CreateBuffer(m_buffer, vertices, sizeof(vertices));
		CreateVAO(m_vao, m_buffer);
	}

	void LFlareHandler::UpdatePositions(const glm::vec3& lightSource,
		const glm::mat4& view, const glm::mat4& projection)
	{
		//m_flarePositions[0] = SunScreenCoord(lightSource, view, projection);
		auto sun = SunScreenCoord(lightSource, view, projection);
		if (sun.has_value())
		{
			m_visible = true;
			m_flarePositions[0] = sun.value();
			glm::vec2 suntoCenter = m_flarePositions[0] - m_center;
			float distance = glm::distance(sun.value(), glm::vec2(0.5f, 0.5f));
			m_brightness = (1.0f - (distance) / 0.6f);
			if (m_brightness < 0.6f)
			{
				m_visible = false;
				return;
			}
			CalculateFlarePositions(suntoCenter, sun.value());
		}
		else m_visible = false;
	}

	void LFlareHandler::CalculateFlarePositions(const glm::vec2& suntoCenter, const glm::vec2& sunCoords)
	{
		for (uint32_t i = 0; i < NUM_FLARE_TEXTURES - 1; ++i)
		{
			glm::vec2 direction(suntoCenter);
			direction *= static_cast<float>(i) * m_spacing;
			m_flarePositions[i + 1] = sunCoords + direction;
		}
	}

	std::optional<glm::vec2> LFlareHandler::SunScreenCoord(const glm::vec3& lightSource, 
		const glm::mat4& view, const glm::mat4& projection)
	{
		glm::vec4 v4SunPos(lightSource.x, lightSource.y, lightSource.z, 1.0f);
		glm::vec4 projected = projection * view * v4SunPos;

		if (projected.w <= 0)
		{
			return std::optional<glm::vec2>{};
		}
		float x = (projected.x / projected.w + 1.0f) / 2.0f;
		float y = 1.0f - ((projected.y / projected.w + 1.0f) / 2.0f);

		return glm::vec2(x, y);
	}

	void LFlareHandler::CreateBuffer(Buffer& buffer, glm::vec2* vertices, uint32_t size)
	{
		buffer.Fill(size, vertices, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
	}
	void LFlareHandler::CreateVAO(VAO& vao, Buffer& buffer)
	{
		buffer.Bind(GL_ARRAY_BUFFER);

		vao.Bind();

		vao.Enable(0);
		vao.Enable(1);

		vao.VAPtr(0, 2, GL_FLOAT, 4 * sizeof(float), nullptr);
		vao.VAPtr(1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

}