#ifndef BIOME_MAP_HEADER
#define BIOME_MAP_HEADER

#include <glm/glm.hpp>

#include "../biome.h"
#include "../../cdata/cvec2.h"
#include "../../../block/block.h"

namespace minecraft
{
	namespace biome
	{
		class BiomeMap
		{
		public:
			struct BCCorners
			{
				glm::vec2 nn;
				glm::vec2 np;
				glm::vec2 pn;
				glm::vec2 pp;
			};
			struct GradientVectors
			{
				glm::vec2 nn;
				glm::vec2 np;
				glm::vec2 pn;
				glm::vec2 pp;
			};
			struct DifferenceVectors
			{
				glm::vec2 nn;
				glm::vec2 np;
				glm::vec2 pn;
				glm::vec2 pp;
			};
			explicit BiomeMap(void) = default;
			BiomeMap& operator=(const BiomeMap&) = default;
			explicit BiomeMap(signed int seed);
		public:
			// determines what biome a block is in
			const biome_t Biome(const glm::vec2& blockCoordxz, const BCCorners& corners, const GradientVectors& gv);
			// calculates the difference vectors for the "perlin noise" style system
			const DifferenceVectors DVectors(const glm::vec2& blockwCoordXZ, const BCCorners& corners);
			// calculates the gradient vectors used to calculate the noise values
			const GradientVectors GVectors(BCCorners& corners);
			// calculates the positions of the biome cell corners
			const BCCorners BiomeCellCorners(const WVec2& wcc);

			const float Test(const glm::vec2& blockCoordxz, const BCCorners& corners, const GradientVectors& gv);
		private:
			const signed int BIOME_MAP_CELL_DIMENSION;
			float BIOME_NOISE_VALUES[2];

			// calculates the negative-x and negative-z corner (used to calculate the other corners)
			glm::vec2 NegCornerOfBiomeMap(signed int x, signed int z);
			// generates one gradient vector at a corner
			glm::vec2 GenerateGVector(glm::vec2& corner);
			// with the noise value, the function will determine which biome the block is in
			biome_t DetermineBiome(float av);

			__forceinline float Lerp(glm::vec2 a, glm::vec2 b, float ref)
			{
				return a.y + ((ref - a.x) / (b.x - a.x)) * (b.y - a.y);
			}
		private:
			signed int m_seed;
		};
	}
}

#endif
