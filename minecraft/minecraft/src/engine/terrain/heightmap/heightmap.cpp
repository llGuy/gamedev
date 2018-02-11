#include "heightmap.h"

namespace minecraft
{
	namespace terrain
	{
		Heightmap::Heightmap(const signed int seed)
			: m_noiseGenerator(seed, 128), COEFF(8.0f)
		{
		}
		const signed int Heightmap::Height(const glm::vec2& blockCoord, const pnoise::PNoise::CellCorners& cc,
			const pnoise::PNoise::GradientVectors& gv, signed int biomeMapHeight)
		{
			return m_noiseGenerator.Noise(blockCoord, cc, gv, COEFF) * biomeMapHeight;
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