#include "terrain.h"

namespace minecraft
{
	namespace terrain
	{
		Terrain::Terrain(const int32_t seed)
			: m_biomeHandler(seed), m_heightmaps{ Heightmap(seed, 55, 30), // multiply by different numbers so that each biome gets different generation
													Heightmap(seed * 23, 70, 30),
													Heightmap(seed * 41, 110, 35)}
													//Heightmap(seed * 91, 80, 70) } 
													//Heightmap(seed * 11, 140, 60) }

													//Heightmap(seed * 54), 
													//Heightmap(seed * 40) }
		{
		}
		const int32_t Terrain::Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc,
			pnoise::PNoise::GradientVectors& gv, biome::biome_t biomeType, float biomeNoise)
		{
			int32_t height;
			// range that will be used to determine the interpolated
			// height in between the biomes
			Range range = InRange(biomeNoise);
			if (range.biome != -0xff)
			{
				// height in current biome
				height = m_heightmaps[static_cast<uint32_t>(biomeType)].Height(blockCoord, cc, gv);
				// height in other biome
				pnoise::PNoise::GradientVectors otherGV = m_heightmaps[range.biome].GVectors(cc);
				int32_t otherHeight = m_heightmaps[range.biome].Height(blockCoord, cc, otherGV);

				float indicator = I(range.diff, m_biomeHandler.BiomeNoiseValue(range.biome));

				if(range.diff > 0.0f) height = static_cast<float>(otherHeight) * (indicator) + height * (1.0f - indicator);
				else height = static_cast<float>(otherHeight) * (1.0f - indicator) + height * (indicator);
			}
			else height = m_heightmaps[static_cast<uint32_t>(biomeType)].Height(blockCoord, cc, gv);

			return height;
		}
		const biome::biome_t Terrain::Biome(glm::vec2 v, pnoise::PNoise::CellCorners& bcc, 
			pnoise::PNoise::GradientVectors& gv, float* noise)
		{
			return m_biomeHandler.Biome(v, bcc, gv, noise);
		}

		const int32_t Terrain::Height(const glm::vec2& blockCoords, const pnoise::PNoise::CellCorners& cc, biome::biome_t b)
		{
			pnoise::PNoise::GradientVectors gv = m_heightmaps[static_cast<uint32_t>(b)].GVectors(cc);
			return m_heightmaps[static_cast<uint32_t>(b)].Height(blockCoords, cc, gv);
		}

		const pnoise::PNoise::DifferenceVectors Terrain::DVectors(const glm::vec2& blockCoord, 
			const pnoise::PNoise::CellCorners& cc, Terrain::choice_t c, biome::biome_t b)
		{
			switch (c)
			{
			case Terrain::choice_t::HM: return m_heightmaps[static_cast<uint32_t>(b)].DVectors(blockCoord, cc);
			case Terrain::choice_t::BM: return m_biomeHandler.DVectors(blockCoord, cc);
			default: return pnoise::PNoise::DifferenceVectors();
			}
		}
		const pnoise::PNoise::GradientVectors Terrain::GVectors(pnoise::PNoise::CellCorners& cc, Terrain::choice_t c, biome::biome_t b)
		{
			switch (c)
			{
			case Terrain::choice_t::HM: return m_heightmaps[static_cast<uint32_t>(b)].GVectors(cc);
			case Terrain::choice_t::BM: return m_biomeHandler.GVectors(cc);
			default: return pnoise::PNoise::GradientVectors();
			}
		}
		const pnoise::PNoise::CellCorners Terrain::CellCorners(const WVec2& chunkCoord, Terrain::choice_t c)
		{
			switch (c)
			{
				// all the heightmap cell corners are the same.
			case Terrain::choice_t::HM: return m_heightmaps[0].HeightmapCellCorners(chunkCoord);
			case Terrain::choice_t::BM: return m_biomeHandler.BiomeMapCellCorners(chunkCoord);
			default: return pnoise::PNoise::CellCorners();
			}
		}
		const int32_t Terrain::BiomeMaxHeight(const biome::biome_t b)
		{
			return m_biomeHandler.MaxBiomeHeight(b);
		}
		const int32_t Terrain::BiomeOffset(const biome::biome_t b)
		{
			return m_biomeHandler.BiomeOffset(b);
		}
		const Block::block_t Terrain::BlockType(const biome::biome_t& b, int32_t maxH, int32_t y)
		{
			return m_biomeHandler.BlockType(b, maxH, y);
		}
	}
}