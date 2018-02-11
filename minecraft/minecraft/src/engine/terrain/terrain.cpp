#include "terrain.h"

namespace minecraft
{
	namespace terrain
	{
		Terrain::Terrain(const signed int seed)
			: m_heightMap(seed), m_biomeHandler(seed)
		{
		}
		const signed int Terrain::Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc,
			pnoise::PNoise::GradientVectors& gv, signed int biomeMapHeight)
		{
			return m_heightMap.Height(blockCoord, cc, gv, biomeMapHeight);
		}
		const biome::biome_t Terrain::Biome(glm::vec2 v, pnoise::PNoise::CellCorners& bcc, 
			pnoise::PNoise::GradientVectors& gv)
		{
			return m_biomeHandler.Biome(v, bcc, gv);
		}

		const pnoise::PNoise::DifferenceVectors Terrain::DVectors(const glm::vec2& blockCoord, 
			const pnoise::PNoise::CellCorners& cc, Terrain::choice_t c)
		{
			switch (c)
			{
			case Terrain::choice_t::HM: return m_heightMap.DVectors(blockCoord, cc);
			case Terrain::choice_t::BM: return m_biomeHandler.DVectors(blockCoord, cc);
			default: return pnoise::PNoise::DifferenceVectors();
			}
		}
		const pnoise::PNoise::GradientVectors Terrain::GVectors(pnoise::PNoise::CellCorners& cc, Terrain::choice_t c)
		{
			switch (c)
			{
			case Terrain::choice_t::HM: return m_heightMap.GVectors(cc);
			case Terrain::choice_t::BM: return m_biomeHandler.GVectors(cc);
			default: return pnoise::PNoise::GradientVectors();
			}
		}
		const pnoise::PNoise::CellCorners Terrain::CellCorners(const WVec2& chunkCoord, Terrain::choice_t c)
		{
			switch (c)
			{
			case Terrain::choice_t::HM: return m_heightMap.HeightmapCellCorners(chunkCoord);
			case Terrain::choice_t::BM: return m_biomeHandler.BiomeMapCellCorners(chunkCoord);
			default: return pnoise::PNoise::CellCorners();
			}
		}
		const signed int Terrain::BiomeMaxHeight(const biome::biome_t b)
		{
			return m_biomeHandler.MaxBiomeHeight(b);
		}
		const signed int Terrain::BiomeOffset(const biome::biome_t b)
		{
			return m_biomeHandler.BiomeOffset(b);
		}
		const Block::block_t Terrain::BlockType(const biome::biome_t& b, signed int maxH, signed int y)
		{
			return m_biomeHandler.BlockType(b, maxH, y);
		}
	}
}