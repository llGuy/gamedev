#ifndef BIOME_HANDLER_HEADER
#define BIOME_HANDLER_HEADER

#include "../../block/block.h"
#include "biomemap/biomemap.h"
#include "biome.h"
#include "plains/plains.h"
#include "desert/desert.h"

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
			const BiomeMap::BCCorners BiomeMapCellCorners(const WVec2& c);
			const BiomeMap::GradientVectors GVectors(BiomeMap::BCCorners& bcc);
			const BiomeMap::DifferenceVectors DVectors(const glm::vec2&& v, const BiomeMap::BCCorners& bcc);
			const biome_t Biome(glm::vec2&& v, BiomeMap::BCCorners& bcc, BiomeMap::GradientVectors& dv);
			const signed int MaxBiomeHeight(const biome_t& b);
			const signed int BiomeOffset(const biome_t& b);

			const Block::block_t BlockType(const biome_t& b, signed int maxH, signed int y);
		private:
			const Block::block_t PlainsBlockType(signed int maxH, signed int y);
			const Block::block_t DesertBlockType(signed int maxH, signed int y);
		private:
			BiomeMap m_biomeMap;
			PlainData m_plainsData;
			DesertData m_desertData;
		};
	}
}

#endif