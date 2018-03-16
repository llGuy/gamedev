#ifndef TERRAIN_HEADER
#define TERRAIN_HEADER

#include "../block/block.h"
#include "heightmap/heightmap.h"
#include "biome/biome_handler.h"

namespace minecraft
{
	namespace terrain
	{
		class Terrain
		{
		public:
			enum class choice_t
				: bool
			{
				HM,

				BM
			};

			explicit Terrain(void) = default;
			explicit Terrain(const int32_t seed);

			const int32_t Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc,
				pnoise::PNoise::GradientVectors& gv, biome::biome_t b, float biomeNoise);
			const biome::biome_t Biome(glm::vec2 v, pnoise::PNoise::CellCorners& bcc, pnoise::PNoise::GradientVectors& dv, float* noise);

			const pnoise::PNoise::DifferenceVectors DVectors(const glm::vec2& blockCoord, 
				const pnoise::PNoise::CellCorners& cc, choice_t c, biome::biome_t b = biome::biome_t::INV);
			const pnoise::PNoise::GradientVectors GVectors(pnoise::PNoise::CellCorners& cc, 
				choice_t c, biome::biome_t b = biome::biome_t::INV);
			const pnoise::PNoise::CellCorners CellCorners(const WVec2& chunkCoord, choice_t c);

			const int32_t BiomeMaxHeight(const biome::biome_t b);
			const int32_t BiomeOffset(const biome::biome_t b);

			const Block::block_t BlockType(const biome::biome_t& b, int32_t maxH, int32_t y);
		private:
			const int32_t Height(const glm::vec2& blockCoords, const pnoise::PNoise::CellCorners& cc, biome::biome_t b);
			
			struct Range
			{
				float diff;
				int32_t biome;
			};

			__forceinline const Range InRange(const float noise)
			{
				Range ret;
				for (uint32_t i = 1; i < m_biomeHandler.BiomeNoiseValuesArrSize(); ++i)
				{
					const float& bnoiseValue = m_biomeHandler.BiomeNoiseValue(i);
					float diff = fabs(noise - bnoiseValue);
					if (diff < 2.0f)
					{
						ret.diff = (noise > bnoiseValue) ? +diff : -diff;
						ret.biome = (ret.diff < 0.0f) ? i : i - 1;
						return ret;
					}
				}
				return { -99.99f, -0xff};
			}
		private:
			biome::BiomeHandler m_biomeHandler;
			//Heightmap m_heightMap;

			// testing
			Heightmap m_heightmaps[static_cast<uint32_t>(biome::biome_t::INV)];

		};
	}
}

#endif