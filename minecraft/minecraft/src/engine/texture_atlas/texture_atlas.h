#ifndef TEXTURE_ATLAS_HEADER
#define TEXTURE_ATLAS_HEADER

#include "../../texture/texture.h"
#include "tile.h"

namespace minecraft
{
	class TextureAtlas
	{
	public:
		explicit TextureAtlas(void) = default;
		// takes in the width as in the number of 
		// tiles in the x axis
		explicit TextureAtlas(float width, float height, const std::string& file);

		Texture* Tex(void);
		TextureAtlasTile Tile(float ind);
		TextureAtlasTile Tile(const glm::vec2& c);
	public:
		void Bind(uint32_t unit);
		void Init(void);
	private:
		Texture m_texture;
		float m_tilesWidth;
		float m_tilesHeight;
	};
}

#endif