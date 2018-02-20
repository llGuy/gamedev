#ifndef BIOME_HANDLER_HEADER
#define BIOME_HANDLER_HEADER

#include "../../block/block.h"
#include "biome.h"
#include "plains/plains.h"
#include "desert/desert.h"
#include "../../noise/noise.h"
#include "mountain/mountains.h"
#include "ocean/ocean.h"

namespace minecraft
{
	namespace biome
	{
		class BiomeHandler
		{
		public:
			explicit BiomeHandler(void) = default;
			BiomeHandler& operator=(const BiomeHandler&) = default;
			explicit BiomeHandler(int32_t seed);
		public:
			const pnoise::PNoise::CellCorners BiomeMapCellCorners(const WVec2& c);
			const pnoise::PNoise::GradientVectors GVectors(pnoise::PNoise::CellCorners& bcc);
			const pnoise::PNoise::DifferenceVectors DVectors(const glm::vec2& v, const pnoise::PNoise::CellCorners& bcc);
			const biome_t Biome(glm::vec2& v, pnoise::PNoise::CellCorners& bcc, pnoise::PNoise::GradientVectors& dv);
			const int32_t MaxBiomeHeight(const biome_t& b);
			const int32_t BiomeOffset(const biome_t& b);

			const Block::block_t BlockType(const biome_t& b, int32_t maxH, int32_t y);
		private:
			const Block::block_t PlainsBlockType(int32_t maxH, int32_t y);
			const Block::block_t DesertBlockType(int32_t maxH, int32_t y);
			const Block::block_t MountainsBlockType(int32_t maxH, int32_t y);
			const Block::block_t OceanBlockType(int32_t maxH, int32_t y);
		private:
			const float COEFF;
			pnoise::PNoise m_noiseGenerator;
			PlainData m_plainsData;
			DesertData m_desertData;
			MountainsData m_mountains;
			OceanData m_oceanData;
		};
	}
}

#endif