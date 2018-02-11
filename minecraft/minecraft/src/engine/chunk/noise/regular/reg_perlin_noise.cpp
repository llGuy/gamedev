#include "reg_perlin_noise.h"
#include <iostream>
#include <math.h>

namespace minecraft
{
	namespace chunk
	{
		namespace noise
		{
			Reg_PerlinNoise::Reg_PerlinNoise(signed int seed)
			{
				m_seed = seed;
			}
			float Reg_PerlinNoise::Height(const glm::vec2& blockwCoordXZ, const CCorners& corners, const GradientVectors& gv, signed int mh)
			{
				DifferenceVectors dv = DVectors(blockwCoordXZ / 10000.0f, corners);

				float dotnn = cos(glm::dot(gv.nn, dv.nn));
				float dotnp = cos(glm::dot(gv.np, dv.np));
				float dotpn = cos(glm::dot(gv.pn, dv.pn));
				float dotpp = cos(glm::dot(gv.pp, dv.pp));

				float nx = Lerp(glm::vec2(corners.nn.x, dotnn), glm::vec2(corners.pn.x, dotpn), blockwCoordXZ.x);
				float px = Lerp(glm::vec2(corners.np.x, dotnp), glm::vec2(corners.pp.x, dotpp), blockwCoordXZ.x);
				
				float av = Lerp(glm::vec2(corners.np.y, px), glm::vec2(corners.nn.y, nx), blockwCoordXZ.y);

				return av * mh;
			}
			const PerlinNoise::GradientVectors Reg_PerlinNoise::GVectors(const CCorners& coords)
			{
				return { GenerateGradientVector(coords.nn), GenerateGradientVector(coords.np),
					GenerateGradientVector(coords.pn), GenerateGradientVector(coords.pp) };
			}
			const glm::vec2 Reg_PerlinNoise::GenerateGradientVector(const glm::vec2& corner)
			{
				signed int h1 = std::hash<signed int>()(static_cast<signed int>(corner.x * 0x123a)) % 0xffff;
				signed int h2 = std::hash<signed int>()(static_cast<signed int>(corner.y * 0x12fb)) % 0xffff;
				signed int hash = h1 + h2;
				srand(hash + m_seed);
				
				// random non-normalized direction

				float rx = static_cast<float>(rand() % 0xff);
				float rz = static_cast<float>(rand() % 0xff);

				// normalized direction * magnitude of the dimension of the chunk
				// the floats are in between -0.5f and 0.5f
				return glm::normalize(glm::vec2((rx / 0xff) - 0.5f, (rz / 0xff) - 0.5f)) * 16.0f;
			}
			const PerlinNoise::DifferenceVectors Reg_PerlinNoise::DVectors(const glm::vec2& blockwCoordXZ, const CCorners& corners)
			{
				glm::vec2 nn = blockwCoordXZ - corners.nn;
				glm::vec2 np = blockwCoordXZ - corners.np;
				glm::vec2 pn = blockwCoordXZ - corners.pn;
				glm::vec2 pp = blockwCoordXZ - corners.pp;
				return { nn, np, pn, pp };
			}
		}
	}
}