#include "block.h"

const uint32_t Block::AVAILABLE_TEXTURES = Block::block_t::INV;

const TextureData Block::BLOCK_TEXTURE_DATA[block_t::INV]
{
	{glm::vec3(1.0f, 1.0f, 1.0f)},
	{glm::vec3(2.0f, 2.0f, 2.0f)},
	{glm::vec3(0.0f, 3.0f, 2.0f)},
	{glm::vec3(17.0f, 17.0f, 17.0f)},
	{glm::vec3(18.0f, 18.0f, 18.0f)},
	{glm::vec3(21.0f, 20.0f, 21.0f)},
	{glm::vec3(53.0f, 53.0f, 53.0f)},
	{glm::vec3(207.0f, 207.0f, 207.0f)}
};

Block::Block(const CCoord& cc, const block_t& bt)
	: m_cc(cc), m_bt(bt), m_valid(true)
{
}

Block::Block(const CCoord&& cc, const block_t&& bt)
	: m_cc(cc), m_bt(bt)
{
}

glm::vec3 Block::WPos(const WVec2 chunkCoordinate, int32_t y, const WVec2 negativeCornerWPos) const
{
	CVec2 blockPosOnChunk = ExtrCPos();
	return glm::vec3(negativeCornerWPos.x + blockPosOnChunk.x, y,
		negativeCornerWPos.z + blockPosOnChunk.z); // +0.5f;
} 

CVec2 Block::ExtrCPos(void) const
{
	return { static_cast<uint8_t>(m_cc.cc >> 4u), 
		static_cast<uint8_t>(m_cc.cc & 0x0fu) };
}

const Block::block_t Block::BlockType(void) const
{
	return m_bt;
}

const TextureData& Block::TextureD(void)
{
	return BLOCK_TEXTURE_DATA[m_bt];
}

uint16_t& Block::VIndex(void)
{
	return m_vIndex;
}

bool& Block::Valid(void)
{
	return m_valid;
}