#ifndef PERLIN_NOISE_CELL_HEADER
#define PERLIN_NOISE_CELL_HEADER
#include <glm/glm.hpp>

namespace dawn { namespace terrain {
		
		struct PerlinNoiseCell
		{
			// n = negative
			// p = positive
			// they represent corners of a square
			// negative x, negative y or z etc

			glm::vec2 nnCorner;
			glm::vec2 ppCorner;
		};

		struct CellCornerVectors
		{
			glm::vec2 nn;
			glm::vec2 np;
			glm::vec2 pn;
			glm::vec2 pp;
		};

		using CellGradientVectors = CellCornerVectors;
		using CellDistanceVectors = CellCornerVectors;
		using NoiseValue = float;

	} 
}

#endif