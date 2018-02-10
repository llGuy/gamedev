#include "block.h"

const unsigned int Block::AVAILABLE_TEXTURES = 
	static_cast<unsigned int>(Block::block_t::INV);

const TextureData Block::BLOCK_TEXTURE_DATA[static_cast<unsigned int>(block_t::INV)]
{
	{glm::vec3(1.0f, 1.0f, 1.0f)},
	{glm::vec3(2.0f, 2.0f, 2.0f)},
	{glm::vec3(0.0f, 3.0f, 2.0f)},
	{glm::vec3(17.0f, 17.0f, 17.0f)},
	{glm::vec3(18.0f, 18.0f, 18.0f)}
};

Block::Block(const CCoord& cc, const block_t& bt)
	: m_cc(cc), m_bt(bt), m_textureData(Block::BLOCK_TEXTURE_DATA[static_cast<unsigned int>(bt)])
{
}

Block::Block(const CCoord&& cc, const block_t&& bt)
	: m_cc(cc), m_bt(bt), m_textureData(Block::BLOCK_TEXTURE_DATA[static_cast<unsigned int>(bt)])
{
}

glm::vec3 Block::WPos(const WVec2 chunkCoordinate, signed int y, const WVec2 negativeCornerWPos) const
{
	CVec2 blockPosOnChunk = ExtrCPos();
	return glm::vec3(negativeCornerWPos.x + blockPosOnChunk.x, y,
		negativeCornerWPos.z + blockPosOnChunk.z);
} 

CVec2 Block::ExtrCPos(void) const
{
	return { static_cast<unsigned char>(m_cc.cc >> 4), static_cast<unsigned char>(m_cc.cc & 0x0f) };
}

const Block::block_t Block::BlockType(void) const
{
	return m_bt;
}

const TextureData& Block::TextureD(void)
{
	return m_textureData;
}