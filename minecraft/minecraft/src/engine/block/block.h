#ifndef BLOCK_HEADER
#define BLOCK_HEADER

#include <glm/glm.hpp>

#include "../../texture/texture.h"
#include "../chunk/cdata/cvec2.h"

/* the coordinate inside the chunk */
/* is stored in an unsigned char to make it RAM efficient */
struct CCoord
{
	uint8_t cc;
};
struct TextureData
{
	// the top, sides, and bottom texture coordinates are stored in vec3
	glm::vec3 topSidesBottom;
};

class Block
{
public:
	// types of blocks available
	enum block_t
		: uint8_t
	{
		STONE, DIRT, GRASS, BEDROCK, SAND, OAK_LOG, LEAVES, WATER, OAK_PLANKS, GLASS, COBBLE, INV /* will add more types of blocks */
	};

	static const uint32_t AVAILABLE_TEXTURES;
	static const TextureData BLOCK_TEXTURE_DATA[block_t::INV];
public:
	explicit Block(void) = default;
	explicit Block(const CCoord& cc, const block_t& bt);
	explicit Block(const CCoord&& cc, const block_t&& bt);
public:
	/* getter */
	glm::vec3 WPos(const WVec2 chunkCoordinate, int32_t y, const WVec2 negativeCornerWPos) const;
	// from the character that stores the block coordinate in chunk
	// extract the coordinates
	CVec2 ExtrCPos(void) const;
	const block_t BlockType(void) const;
	const TextureData& TextureD(void);
	// index in the 
	uint16_t& VIndex(void);
	bool& Valid(void);
private:
	uint16_t m_vIndex;
	CCoord m_cc;
	block_t m_bt;
	bool m_valid;
};

#endif