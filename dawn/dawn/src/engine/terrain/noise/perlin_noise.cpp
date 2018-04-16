#include "perlin_noise.h"

namespace dawn { namespace terrain {


		PerlinNoiseGenerator::PerlinNoiseGenerator(int32_t dimension)
			: m_cellDimension(dimension)
		{
		}

		PerlinNoiseCell PerlinNoiseGenerator::CalculateCell(const glm::ivec2& worldcoord) const
		{

			glm::vec2 nn();
		}
		CellGradientVectors PerlinNoiseGenerator::CalculateGradientVectors(const glm::vec2& coord, const PerlinNoiseCell& cell) const
		{

		}
		CellDistanceVectors PerlinNoiseGenerator::CalculateDistanceVectors(const glm::vec2& coord, const PerlinNoiseCell& cell) const
		{

		}
		NoiseValue PerlinNoiseGenerator::CalculateNoiseValue(const CellGradientVectors&, const CellDistanceVectors&) const
		{

		}
		
	}
}