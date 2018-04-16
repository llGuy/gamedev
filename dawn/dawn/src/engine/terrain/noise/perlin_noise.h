#ifndef PERLIN_NOISE_GENERATOR_HEADER
#define PERLIN_NOISE_GENERATOR_HEADER
#include "perlin_noise_cell.h"

namespace dawn { namespace terrain {

		class PerlinNoiseGenerator
		{
		public:
			PerlinNoiseGenerator(int32_t dimension);
			~PerlinNoiseGenerator(void) = default;

			PerlinNoiseCell CalculateCell(const glm::ivec2& worldcoord) const;
			CellGradientVectors CalculateGradientVectors(const glm::vec2& coord, const PerlinNoiseCell& cell) const;
			CellDistanceVectors CalculateDistanceVectors(const glm::vec2& coord, const PerlinNoiseCell& cell) const;
			NoiseValue CalculateNoiseValue(const CellGradientVectors&, const CellDistanceVectors&) const;
		private:
			const int32_t m_cellDimension;
		};

	} 
}

#endif