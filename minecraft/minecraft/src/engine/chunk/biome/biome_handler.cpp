#include "biome_handler.h"

namespace minecraft
{
	namespace biome
	{
		BiomeHandler::BiomeHandler(signed int seed)
			: m_biomeMap(seed), m_plainsData(), m_desertData()
		{
		}
		const BiomeMap::BCCorners BiomeHandler::BiomeMapCellCorners(const WVec2& c)
		{
			return m_biomeMap.BiomeCellCorners(c);
		}
		const BiomeMap::GradientVectors BiomeHandler::GVectors(BiomeMap::BCCorners& bcc)
		{
			return m_biomeMap.GVectors(bcc);
		}
		const BiomeMap::DifferenceVectors BiomeHandler::DVectors(const glm::vec2&& v, const BiomeMap::BCCorners& bcc)
		{
			return m_biomeMap.DVectors(v, bcc);
		}
		const biome_t BiomeHandler::Biome(glm::vec2&& v,
			BiomeMap::BCCorners& bcc, BiomeMap::GradientVectors& dv)
		{
			return m_biomeMap.Biome(v, bcc, dv);
		}

		const Block::block_t BiomeHandler::BlockType(const biome_t& b, signed int maxH, signed int y)
		{
			switch (b)
			{
			case biome_t::PLAINS: return PlainsBlockType(maxH, y); break;
			case biome_t::DESERT: return DesertBlockType(maxH, y); break;
			default: return Block::block_t::INV;
			}
		}
		const signed int BiomeHandler::MaxBiomeHeight(const biome_t& b)
		{
			switch (b)
			{
			case biome_t::PLAINS: return m_plainsData.MAX_HEIGHT; break;
			case biome_t::DESERT: return m_desertData.MAX_HEIGHT; break;
			default: return 0;
			}
		}
		const signed int BiomeHandler::BiomeOffset(const biome_t& b)
		{
			switch (b)
			{
			case biome_t::PLAINS: return m_plainsData.OFFSET; break;
			case biome_t::DESERT: return m_desertData.OFFSET; break;
			default: return 0;
			}
		}
		const Block::block_t BiomeHandler::PlainsBlockType(signed int maxH, signed int y)
		{
			if (y == maxH) return Block::block_t::GRASS;
			else if (y >= maxH * m_plainsData.DIRT_LEVEL) return Block::block_t::DIRT;
			else if (y >= maxH * m_plainsData.STONE_LEVEL) return Block::block_t::STONE;
			else return Block::block_t::BEDROCK;
		}
		const Block::block_t BiomeHandler::DesertBlockType(signed int maxH, signed int y)
		{
			if (y >= maxH * m_desertData.SAND_LEVEL) return Block::block_t::SAND;
			else if (y >= maxH * m_desertData.DIRT_LEVEL) return Block::block_t::DIRT;
			else if (y >= maxH * m_desertData.STONE_LEVEL) return Block::block_t::STONE;
			else return Block::block_t::BEDROCK;
		}
	}
}