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

			explicit PNoise(int32_t seed, const int32_t cellDim)
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

				return (av + 1.0f) / 2.0f;
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
				WVec2 chunkCoordWorld;
				chunkCoordWorld.x = chunkCoord.x * 16;
				chunkCoordWorld.z = chunkCoord.z * 16;

				WVec2 cellCoordwv2 = CellCoord(chunkCoordWorld);
				glm::vec2 cellCoordWorld;
				cellCoordWorld.x = static_cast<float>(cellCoordwv2.x * CELL_DIMENSION);
				cellCoordWorld.y = static_cast<float>(cellCoordwv2.z * CELL_DIMENSION);

				float half = static_cast<float>(CELL_DIMENSION / 2.0f);

				return
				{
					glm::vec2(cellCoordWorld.x - half - 0.5f, cellCoordWorld.y - half - 0.5f),
					glm::vec2(cellCoordWorld.x - half - 0.5f, cellCoordWorld.y + half - 0.5f),
					glm::vec2(cellCoordWorld.x + half - 0.5f, cellCoordWorld.y - half - 0.5f),
					glm::vec2(cellCoordWorld.x + half - 0.5f, cellCoordWorld.y + half - 0.5f)
				};
			}
			const WVec2 CellCoord(const WVec2& chunkCoord)
			{
				WVec2 cellCoord;
				cellCoord.x = chunkCoord.x == 0 ? 0 : (abs(chunkCoord.x) + CELL_DIMENSION / 2) * (chunkCoord.x / abs(chunkCoord.x)) / CELL_DIMENSION;
				cellCoord.z = chunkCoord.z == 0 ? 0 : (abs(chunkCoord.z) + CELL_DIMENSION / 2) * (chunkCoord.z / abs(chunkCoord.z)) / CELL_DIMENSION;
				return cellCoord;
			}
		protected:
			const glm::vec2 GenerateGVector(const glm::vec2& corner)
			{
				int32_t hashComp1 = std::hash<int32_t>()(static_cast<int32_t>(corner.x * 0x12a));
				int32_t hashComp2 = std::hash<int32_t>()(static_cast<int32_t>(corner.y * 0xf12));
				int32_t hash = hashComp1 + hashComp2 + SEED;
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
			const int32_t SEED;
			const int32_t CELL_DIMENSION;
		};
	}
}

#endif