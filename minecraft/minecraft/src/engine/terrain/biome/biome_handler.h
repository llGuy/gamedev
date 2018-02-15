#ifndef BIOME_HANDLER_HEADER
#define BIOME_HANDLER_HEADER

#include "../../block/block.h"
#include "biome.h"
#include "plains/plains.h"
#include "desert/desert.h"
#include "../../noise/noise.h"
#include "mountain/mountains.h"

namespace minecraft
{
	namespace biome
	{
		class BiomeHandler
		{
		public:
			explicit BiomeHandler(void) = default;
			BiomeHandler& operator=(const BiomeHandler&) = default;
			explicit BiomeHandler(signed int seed);
		public:
			const pnoise::PNoise::CellCorners BiomeMapCellCorners(const WVec2& c);
			const pnoise::PNoise::GradientVectors GVectors(pnoise::PNoise::CellCorners& bcc);
			const pnoise::PNoise::DifferenceVectors DVectors(const glm::vec2& v, const pnoise::PNoise::CellCorners& bcc);
			const biome_t Biome(glm::vec2& v, pnoise::PNoise::CellCorners& bcc, pnoise::PNoise::GradientVectors& dv);
			const signed int MaxBiomeHeight(const biome_t& b);
			const signed int BiomeOffset(const biome_t& b);

			const Block::block_t BlockType(const biome_t& b, signed int maxH, signed int y);
		private:
			const Block::block_t PlainsBlockType(signed int maxH, signed int y);
			const Block::block_t DesertBlockType(signed int maxH, signed int y);
			const Block::block_t MountainsBlockType(signed int maxH, signed int y);
		private:
			const float COEFF;
			pnoise::PNoise m_noiseGenerator;
			PlainData m_plainsData;
			DesertData m_desertData;
			MountainsData m_mountains;
		};
	}
}

#endif