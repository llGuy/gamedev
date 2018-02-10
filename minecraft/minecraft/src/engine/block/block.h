#ifndef BLOCK_HEADER
#define BLOCK_HEADER

#include <glm/glm.hpp>

#include "../../texture/texture.h"
#include "../chunk/cdata/cvec2.h"

/* the coordinate inside the chunk */
/* is stored in an unsigned char to make it RAM efficient */
struct CCoord
{
	unsigned char cc;
};
struct TextureData
{
	// the top, sides, and bottom texture coordinates are stored in vec3
	glm::vec3 topSidesBottom;
};

class Block
{
public:
	enum class BlType
		: unsigned char	
	{
		STONE, DIRT, GRASS, BEDROCK, SAND, INV /* will add more types of blocks */
	};
	//static Texture TEXTURE_ATLAS;
	//static unsigned int m_textures[3];

	static const unsigned int AVAILABLE_TEXTURES;
	static const TextureData BLOCK_TEXTURE_DATA[static_cast<signed int>(BlType::INV)];
public:
	explicit Block(void) = default;
	explicit Block(const CCoord& cc, const BlType& bt);
	explicit Block(const CCoord&& cc, const BlType&& bt);
public:
	/* getter */
	glm::vec3 WPos(const WVec2 chunkCoordinate, signed int y, const WVec2 negativeCornerWPos) const;
	CVec2 ExtrCPos(void) const;
	const BlType BlockType(void) const;
	const TextureData& TextureD(void);
private:
	CCoord m_cc;
	BlType m_bt;
	TextureData m_textureData;
};

#endif