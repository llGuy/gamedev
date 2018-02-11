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
			explicit Heightmap(const signed int seed);
		public:
			const signed int Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc, 
				const pnoise::PNoise::GradientVectors& gv, signed int biomeMapHeight);
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