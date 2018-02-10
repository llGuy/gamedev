#ifndef PERLIN_NOISE
#define PERLIN_NOISE

#include <glm/glm.hpp>
#include "../cdata/cvec2.h"

namespace minecraft
{
	namespace chunk
	{
		namespace noise
		{
			class PerlinNoise
			{
			public:
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
				virtual float Height(const glm::vec2& blockwCoordXZ, 
					const CCorners& coords, const GradientVectors& gv, signed int mh) { return 0.0f; }
				virtual const GradientVectors GVectors(const CCorners& ccoords) { return {}; }
			protected:
				signed int m_seed;
			};
		}
	}
}

#endif