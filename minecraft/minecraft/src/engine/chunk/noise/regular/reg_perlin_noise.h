#ifndef REG_PERLIN_NOISE_HEADER
#define REG_PERLIN_NOISE_HEADER

#include "../perlinnoise.h"

namespace minecraft
{
	namespace chunk
	{
		namespace noise
		{
			class Reg_PerlinNoise
				: public PerlinNoise
			{
			public:
				explicit Reg_PerlinNoise(signed int seed);
			public:
				float Height(const glm::vec2& blockwCoordXZ, const CCorners& coords, const GradientVectors& gv, signed int mh) override;
				const GradientVectors GVectors(const CCorners& ccoords) override;
			private:
				const glm::vec2 GenerateGradientVector(const glm::vec2& corner);
				const DifferenceVectors DVectors(const glm::vec2& blockwCoordXZ, const CCorners& coords);

				__forceinline float Lerp(glm::vec2 a, glm::vec2 b, float ref)
				{
					return a.y + ((ref - a.x) / (b.x - a.x)) * (b.y - a.y);
				}
			};
		}
	}
}

#endif