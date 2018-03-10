#include "texture_atlas.h"

namespace minecraft
{
	TextureAtlas::TextureAtlas(float width, float height, const std::string& file)
		: m_tilesWidth(width), m_tilesHeight(height), m_texture(file)
	{
	}
	void TextureAtlas::Init(void)
	{
		m_texture.Init();
	}
	void TextureAtlas::Bind(uint32_t unit)
	{
		m_texture.Bind(unit);
	}
	Texture* TextureAtlas::Tex(void)
	{
		return &m_texture;
	}
	TextureAtlasTile TextureAtlas::Tile(float ind)
	{
		float center = 0.0007f;
		TextureAtlasTile tat;
		float tdu = glm::mod(ind, m_tilesWidth) / m_tilesWidth;
		float tdv = glm::floor(ind / m_tilesHeight) / m_tilesHeight;
		tat.tx00 = glm::vec2(tdu + center, tdv + center);
		tat.tx01 = glm::vec2(tdu + 1.0f / m_tilesWidth - center, tdv + center);
		tat.tx10 = glm::vec2(tdu + center, tdv + 1.0f / m_tilesHeight - center);
		tat.tx11 = glm::vec2(tdu + 1.0f / m_tilesWidth - center, tdv + 1.0f / m_tilesHeight - center);
		return tat;
	}
	TextureAtlasTile TextureAtlas::Tile(const glm::vec2& c)
	{
		// test
		float index = glm::mod(c.s, m_tilesWidth) + glm::floor(c.t) * m_tilesWidth;
		return Tile(index);
	}
}