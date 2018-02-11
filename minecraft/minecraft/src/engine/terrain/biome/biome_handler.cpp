#include "biome_handler.h"

namespace minecraft
{
	namespace biome
	{
		BiomeHandler::BiomeHandler(signed int seed)
			: m_noiseGenerator(seed, 64), m_plainsData(), m_desertData(), COEFF(2048.0f)
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
			noise *= 10.0f;
			// av is between -10.0f, and 10.0f
			if (noise >= 0.0f) return biome_t::PLAINS;
			return biome_t::DESERT;
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