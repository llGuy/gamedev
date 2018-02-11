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
			explicit Terrain(const signed int seed);

			const signed int Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc,
				pnoise::PNoise::GradientVectors& gv, signed int biomeMapHeight);
			const biome::biome_t Biome(glm::vec2 v, pnoise::PNoise::CellCorners& bcc, pnoise::PNoise::GradientVectors& dv);

			const pnoise::PNoise::DifferenceVectors DVectors(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc, choice_t c);
			const pnoise::PNoise::GradientVectors GVectors(pnoise::PNoise::CellCorners& cc, choice_t c);
			const pnoise::PNoise::CellCorners CellCorners(const WVec2& chunkCoord, choice_t c);        

			const signed int BiomeMaxHeight(const biome::biome_t b);
			const signed int BiomeOffset(const biome::biome_t b);

			const Block::block_t BlockType(const biome::biome_t& b, signed int maxH, signed int y);
		private:
			biome::BiomeHandler m_biomeHandler;
			Heightmap m_heightMap;
		};
	}
}

#endif