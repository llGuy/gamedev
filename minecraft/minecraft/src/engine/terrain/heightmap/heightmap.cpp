#include "heightmap.h"

namespace minecraft
{
	namespace terrain
	{
		Heightmap::Heightmap(const int32_t seed, const int32_t& offset, const int32_t& maxHeight)
			: m_noiseGenerator(seed, 16 * 7), COEFF(16.0f), m_offset(offset), m_maxHeight(maxHeight)
		{
		}
		const int32_t Heightmap::Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc,
			const pnoise::PNoise::GradientVectors& gv)
		{
			return static_cast<int32_t>(m_noiseGenerator.Noise(blockCoord, cc, gv, COEFF) * m_maxHeight) + m_offset;
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