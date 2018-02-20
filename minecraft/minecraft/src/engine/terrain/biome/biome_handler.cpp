#include "biome_handler.h"

namespace minecraft
{
	namespace biome
	{
		BiomeHandler::BiomeHandler(int32_t seed)
			: m_noiseGenerator(seed, 16 * 23), m_plainsData(), m_desertData(), COEFF(2048.0f * 2.0f)
		{
		}
		const pnoise::PNoise::CellCorners BiomeHandler::BiomeMapCellCorners(const WVec2& c)
		{
			return m_noiseGenerator.Corners(c);
		}
		const pnoise::PNoise::GradientVectors BiomeHandler::GVectors(pnoise::PNoise::CellCorners& bcc)
		{
			return m_noiseGenerator.GVectors(bcc);
		}
		const pnoise::PNoise::DifferenceVectors BiomeHandler::DVectors(const glm::vec2& v, const pnoise::PNoise::CellCorners& bcc)
		{
			return m_noiseGenerator.DVectors(v, bcc);
		}
		const biome_t BiomeHandler::Biome(glm::vec2& v,
			pnoise::PNoise::CellCorners& bcc, pnoise::PNoise::GradientVectors& dv)
		{
			float noise = m_noiseGenerator.Noise(v, bcc, dv, COEFF);
			noise *= 100.0f;
			// av is between -10.0f, and 10.0f
			if (noise >= 70.0f) return biome_t::MOUNTAINS;
			else if (noise >= 20.0f) return biome_t::PLAINS;
			else if(noise >= -10.0f) return biome_t::DESERT;
			else return biome_t::OCEAN;
		}

		const Block::block_t BiomeHandler::BlockType(const biome_t& b, int32_t maxH, int32_t y)
		{
			switch (b)
			{
			case biome_t::OCEAN: return OceanBlockType(maxH, y);
			case biome_t::PLAINS: return PlainsBlockType(maxH, y);
			case biome_t::DESERT: return DesertBlockType(maxH, y); 
			case biome_t::MOUNTAINS: return MountainsBlockType(maxH, y); 
			default: return Block::block_t::INV;
			}
		}
		const int32_t BiomeHandler::MaxBiomeHeight(const biome_t& b)
		{
			switch (b)
			{
			case biome_t::OCEAN: return m_oceanData.MAX_HEIGHT;
			case biome_t::PLAINS: return m_plainsData.MAX_HEIGHT; 
			case biome_t::DESERT: return m_desertData.MAX_HEIGHT; 
			case biome_t::MOUNTAINS: return m_mountains.MAX_HEIGHT; 
			default: return 0;
			}
		}
		const int32_t BiomeHandler::BiomeOffset(const biome_t& b)
		{
			switch (b)
			{
			case biome_t::OCEAN: return m_oceanData.MAX_HEIGHT;
			case biome_t::PLAINS: return m_plainsData.OFFSET; 
			case biome_t::DESERT: return m_desertData.OFFSET; 
			case biome_t::MOUNTAINS: return m_mountains.OFFSET; 
			default: return 0;
			}
		}
		const Block::block_t BiomeHandler::PlainsBlockType(int32_t maxH, int32_t y)
		{
			if (y == maxH) return Block::block_t::GRASS;
			else if (y >= maxH * m_plainsData.DIRT_LEVEL) return Block::block_t::DIRT;
			else if (y >= maxH * m_plainsData.STONE_LEVEL) return Block::block_t::STONE;
			else return Block::block_t::BEDROCK;
		}
		const Block::block_t BiomeHandler::DesertBlockType(int32_t maxH, int32_t y)
		{
			if (y >= maxH * m_desertData.SAND_LEVEL) return Block::block_t::SAND;
			else if (y >= maxH * m_desertData.DIRT_LEVEL) return Block::block_t::DIRT;
			else if (y >= maxH * m_desertData.STONE_LEVEL) return Block::block_t::STONE;
			else return Block::block_t::BEDROCK;
		}
		const Block::block_t BiomeHandler::MountainsBlockType(int32_t maxH, int32_t y)
		{
			/* at the top there is stone */
			if (y == maxH && maxH > m_mountains.STONE_TOP * (m_mountains.OFFSET + m_mountains.MAX_HEIGHT)) return Block::block_t::STONE;
			else if (y == maxH) return Block::block_t::GRASS;
			else if (y >= maxH * m_mountains.DIRT_LEVEL) return Block::block_t::DIRT;
			else if (y >= maxH * m_mountains.STONE_LEVEL) return Block::block_t::STONE;
			else return Block::block_t::BEDROCK;
		}
		const Block::block_t BiomeHandler::OceanBlockType(int32_t maxH, int32_t y)
		{
			/* at the top there is stone */
			if (y >= maxH * m_oceanData.SAND_LEVEL) return Block::block_t::SAND;
			else if (y >= maxH * m_oceanData.DIRT_LEVEL) return Block::block_t::DIRT;
			else if (y >= maxH * m_oceanData.STONE_LEVEL) return Block::block_t::STONE;
			else return Block::block_t::BEDROCK;
		}
	}
}