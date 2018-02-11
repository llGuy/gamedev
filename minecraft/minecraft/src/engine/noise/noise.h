#ifndef NOISE_HEADER
#define NOISE_HEADER

#include "../chunk/cdata/cvec2.h"
#include "../terrain/biome/biome.h"
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

namespace minecraft
{
	namespace pnoise
	{
		class PNoise
		{
		private:
			struct V2CornerStruct
			{
				glm::vec2 nn;
				glm::vec2 np;
				glm::vec2 pn;
				glm::vec2 pp;
			};
		public:
			using DifferenceVectors = V2CornerStruct;
			using GradientVectors = V2CornerStruct;
			using CellCorners = V2CornerStruct;

			explicit PNoise(signed int seed, const signed int cellDim)
				: SEED(seed), CELL_DIMENSION(cellDim)
			{
			}

		public:
			const float Noise(const glm::vec2& blockCoord, const CellCorners& cc, const GradientVectors& gv, float coeff) 
			{
				DifferenceVectors dv = DVectors(blockCoord / coeff, cc);

				float dotnn = cos(glm::dot(gv.nn, dv.nn));
				float dotnp = cos(glm::dot(gv.np, dv.np));
				float dotpn = cos(glm::dot(gv.pn, dv.pn));
				float dotpp = cos(glm::dot(gv.pp, dv.pp));

				float nx = Lerp(glm::vec2(cc.nn.x, dotnn), glm::vec2(cc.pn.x, dotpn), blockCoord.x);
				float px = Lerp(glm::vec2(cc.np.x, dotnp), glm::vec2(cc.pp.x, dotpp), blockCoord.x);

				float av = Lerp(glm::vec2(cc.np.y, px), glm::vec2(cc.nn.y, nx), blockCoord.y);

				return av;
			}
			const DifferenceVectors DVectors(const glm::vec2& blockCoord, const CellCorners& cc) 
			{
				return
				{
					blockCoord - cc.nn,
					blockCoord - cc.np,
					blockCoord - cc.pn,
					blockCoord - cc.pp,
				};
			}
			const GradientVectors GVectors(const CellCorners& cc) 
			{
				return
				{
					GenerateGVector(cc.nn),
					GenerateGVector(cc.np),
					GenerateGVector(cc.pn),
					GenerateGVector(cc.pp)
				};
			}
			const CellCorners Corners(const WVec2& chunkCoord) 
			{
				signed int xchunkCenter = chunkCoord.x * 16;
				signed int zchunkCenter = chunkCoord.z * 16;

				glm::vec2 ncorner = CellNCorner(xchunkCenter, zchunkCenter);
				glm::vec2 pcorner = CellPCorner(xchunkCenter, zchunkCenter);

				return
				{
					ncorner,
					glm::vec2(ncorner.x, ncorner.y + CELL_DIMENSION),
					glm::vec2(ncorner.x + CELL_DIMENSION, ncorner.y),
					glm::vec2(ncorner.x + CELL_DIMENSION, ncorner.y + CELL_DIMENSION)
				};
			}
			const glm::vec2 CellNCorner(signed int x, signed int z)
			{
				WVec2 xz = { x, z };
				signed int biomeCellCoordx = xz.x == 0 ? 0 :
					(abs(xz.x) + CELL_DIMENSION / 2) * (xz.x / abs(xz.x)) / CELL_DIMENSION;
				signed int biomeCellCoordz = xz.z == 0 ? 0 :
					(abs(xz.z) + CELL_DIMENSION / 2) * (xz.z / abs(xz.z)) / CELL_DIMENSION;

				return glm::vec2(biomeCellCoordx * CELL_DIMENSION - (static_cast<float>(CELL_DIMENSION / 2 + 0.5f)),
					biomeCellCoordz * CELL_DIMENSION - (static_cast<float>(CELL_DIMENSION / 2 + 0.5f)));
			}
			const glm::vec2 CellPCorner(signed int x, signed int z)
			{
				WVec2 xz = { x, z };
				signed int biomeCellCoordx = xz.x == 0 ? 0 :
					(abs(xz.x) + CELL_DIMENSION / 2) * (xz.x / abs(xz.x)) / CELL_DIMENSION;
				signed int biomeCellCoordz = xz.z == 0 ? 0 :
					(abs(xz.z) + CELL_DIMENSION / 2) * (xz.z / abs(xz.z)) / CELL_DIMENSION;

				return glm::vec2(biomeCellCoordx * CELL_DIMENSION + (static_cast<float>(CELL_DIMENSION / 2 + 0.5f)),
					biomeCellCoordz * CELL_DIMENSION + (static_cast<float>(CELL_DIMENSION / 2 + 0.5f)));
			}
		protected:
			const glm::vec2 GenerateGVector(const glm::vec2& corner)
			{
				signed int hashComp1 = std::hash<signed int>()(static_cast<signed int>(corner.x * 0x12a));
				signed int hashComp2 = std::hash<signed int>()(static_cast<signed int>(corner.y * 0xf12));
				signed int hash = hashComp1 + hashComp2 + SEED;
				srand(hash);

				float rx = static_cast<float>(rand() % 0xff);
				float rz = static_cast<float>(rand() % 0xff);

				/* scaling the values of rx and rz to be between 0.5f and -0.5f */
				return glm::normalize(glm::vec2(glm::vec2((rx / 0xff) - 0.5f, (rz / 0xff) - 0.5f)) * static_cast<float>(CELL_DIMENSION));
			}
		protected:
			__forceinline float Lerp(const glm::vec2& a, const glm::vec2& b, float ref)
			{
				return a.y + ((ref - a.x) / (b.x - a.x)) * (b.y - a.y);
			}
			const signed int SEED;
			const signed int CELL_DIMENSION;
		};
	}
}

#endif