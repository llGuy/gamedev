#include "heightmap.h"

namespace minecraft
{
	namespace terrain
	{
		Heightmap::Heightmap(const int32_t seed)
			: m_noiseGenerator(seed, 16 * 7), COEFF(16.0f)
		{
		}
		const int32_t Heightmap::Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc,
			const pnoise::PNoise::GradientVectors& gv, int32_t biomeMapHeight)
		{
			return static_cast<int32_t>(m_noiseGenerator.Noise(blockCoord, cc, gv, COEFF) * biomeMapHeight);
		}
		const pnoise::PNoise::DifferenceVectors Heightmap::DVectors(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc)
		{
			return m_noiseGenerator.DVectors(blockCoord, cc);
		}
		const pnoise::PNoise::GradientVectors Heightmap::GVectors(const pnoise::PNoise::CellCorners& cc)
		{
			return m_noiseGenerator.GVectors(cc);
		}
		const pnoise::PNoise::CellCorners Heightmap::HeightmapCellCorners(const WVec2& chunkCoord)
		{
			return m_noiseGenerator.Corners(chunkCoord);
		}
	}
}