#ifndef HEIGHTMAP_HEADER
#define HEIGHTMAP_HEADER

#include "../../noise/noise.h"

namespace minecraft
{
	namespace terrain
	{
		class Heightmap
		{
		public:
			explicit Heightmap(void) = default;
			explicit Heightmap(const int32_t seed);
		public:
			const int32_t Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc, 
				const pnoise::PNoise::GradientVectors& gv, int32_t biomeMapHeight);
			const pnoise::PNoise::DifferenceVectors DVectors(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc);
			const pnoise::PNoise::GradientVectors GVectors(const pnoise::PNoise::CellCorners& cc);
			const pnoise::PNoise::CellCorners HeightmapCellCorners(const WVec2& chunkCoord);
		private:
			const float COEFF;
			pnoise::PNoise m_noiseGenerator;
		};
	}
}

#endif