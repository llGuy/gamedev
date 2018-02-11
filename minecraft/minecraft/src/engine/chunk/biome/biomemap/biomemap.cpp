#include "biomemap.h"

namespace minecraft
{
	namespace biome
	{
		BiomeMap::BiomeMap(signed int seed)
			: m_seed(seed), BIOME_MAP_CELL_DIMENSION(128)
		{
		}
		const biome_t BiomeMap::Biome(const glm::vec2& blockCoordxz, const BCCorners& corners, const GradientVectors& gv)
		{
			DifferenceVectors dv = DVectors(blockCoordxz / 2048.0f, corners);

			float dotnn = cos(glm::dot(gv.nn, dv.nn));
			float dotnp = cos(glm::dot(gv.np, dv.np));
			float dotpn = cos(glm::dot(gv.pn, dv.pn));
			float dotpp = cos(glm::dot(gv.pp, dv.pp));

			float nx = Lerp(glm::vec2(corners.nn.x, dotnn), glm::vec2(corners.pn.x, dotpn), blockCoordxz.x);
			float px = Lerp(glm::vec2(corners.np.x, dotnp), glm::vec2(corners.pp.x, dotpp), blockCoordxz.x);

			float av = Lerp(glm::vec2(corners.np.y, px), glm::vec2(corners.nn.y, nx), blockCoordxz.y);
			return DetermineBiome(av);
		}
		const BiomeMap::DifferenceVectors BiomeMap::DVectors(const glm::vec2& blockwCoordXZ, const BCCorners& corners)
		{
			glm::vec2 nn = blockwCoordXZ - corners.nn;
			glm::vec2 np = blockwCoordXZ - corners.np;
			glm::vec2 pn = blockwCoordXZ - corners.pn;
			glm::vec2 pp = blockwCoordXZ - corners.pp;

			return { nn, np, pn, pp };
		}
		const BiomeMap::GradientVectors BiomeMap::GVectors(BiomeMap::BCCorners& corners)
		{
			return { GenerateGVector(corners.nn), GenerateGVector(corners.np),
				GenerateGVector(corners.pn), GenerateGVector(corners.pp) };
		}
		glm::vec2 BiomeMap::GenerateGVector(glm::vec2& corner)
		{
			signed int h1 = std::hash<signed int>()(static_cast<signed int>(corner.x * 0x123a)) % 0xffff;
			signed int h2 = std::hash<signed int>()(static_cast<signed int>(corner.y * 0x12fb)) % 0xffff;
			signed int hash = h1 + h2;
			srand(hash + m_seed);

			float rx = static_cast<float>(rand() % 0xff);
			float rz = static_cast<float>(rand() % 0xff);

			// getting values between -0.5f and 0.5f to normalize and multiply by the magnitude of the biome cell dimension
			return glm::normalize(glm::vec2((rx / 0xff) - 0.5f, (rz / 0xff) - 0.5f)) * static_cast<float>(BIOME_MAP_CELL_DIMENSION);
		}
		const BiomeMap::BCCorners BiomeMap::BiomeCellCorners(const WVec2& wcc)
		{
			signed int xchunkcenter = wcc.x * 16;
			signed int zchunkcenter = wcc.z * 16;

			glm::vec2 negCorner = NegCornerOfBiomeMap(xchunkcenter, zchunkcenter);

			return
			{
				negCorner,
				glm::vec2(negCorner.x, negCorner.y + BIOME_MAP_CELL_DIMENSION),
				glm::vec2(negCorner.x + BIOME_MAP_CELL_DIMENSION, negCorner.y),
				glm::vec2(negCorner.x + BIOME_MAP_CELL_DIMENSION, negCorner.y + BIOME_MAP_CELL_DIMENSION)
			};
		}
		glm::vec2 BiomeMap::NegCornerOfBiomeMap(signed int x, signed int z)
		{
			WVec2 xz = { x, z };
			signed int biomeCellCoordx = xz.x == 0 ? 0 :
				(abs(xz.x) + BIOME_MAP_CELL_DIMENSION / 2) * (xz.x / abs(xz.x)) / BIOME_MAP_CELL_DIMENSION;
			signed int biomeCellCoordz = xz.z == 0 ? 0 :
				(abs(xz.z) + BIOME_MAP_CELL_DIMENSION / 2) * (xz.z / abs(xz.z)) / BIOME_MAP_CELL_DIMENSION;
			
			return glm::vec2(biomeCellCoordx * BIOME_MAP_CELL_DIMENSION - (static_cast<float>(BIOME_MAP_CELL_DIMENSION / 2 + 0.5f)), 
				biomeCellCoordz * BIOME_MAP_CELL_DIMENSION - (static_cast<float>(BIOME_MAP_CELL_DIMENSION / 2 + 0.5f)));
		}
		biome_t BiomeMap::DetermineBiome(float av)
		{
			av *= 10.0f;
			// av is between -10.0f, and 10.0f
			if (av >= 0.0f)
			{
				return biome_t::PLAINS;
			}
			return biome_t::DESERT;
		}
		const float BiomeMap::Test(const glm::vec2& blockCoordxz, const BCCorners& corners, const GradientVectors& gv)
		{
			DifferenceVectors dv = DVectors(blockCoordxz / 2048.0f, corners);

			float dotnn = cos(glm::dot(gv.nn, dv.nn));
			float dotnp = cos(glm::dot(gv.np, dv.np));
			float dotpn = cos(glm::dot(gv.pn, dv.pn));
			float dotpp = cos(glm::dot(gv.pp, dv.pp));

			float nx = Lerp(glm::vec2(corners.nn.x, dotnn), glm::vec2(corners.pn.x, dotpn), blockCoordxz.x);
			float px = Lerp(glm::vec2(corners.np.x, dotnp), glm::vec2(corners.pp.x, dotpp), blockCoordxz.x);

			float av = Lerp(glm::vec2(corners.np.y, px), glm::vec2(corners.nn.y, nx), blockCoordxz.y) * 10.0f;
			return av;
		}
	}
}