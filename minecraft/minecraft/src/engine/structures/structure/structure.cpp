#include "structure.h"

namespace minecraft
{
	namespace structures
	{
		Structure::Structure(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, GenRange sr, WVec2 worldCoord)
			: m_worldChunkCoord(wcc), m_structGenRange(sr), m_structureOriginChunk(originCoordInChunk), 
			m_structureOriginWorld(worldCoord)
		{
		}
		GenRange Structure::GenerateStructRangeAll(void) const
		{
			return GenRange();
		}
		StructCompBYS Structure::GenerateYStripOfStruct(WVec2 worldCoord) 
		{
			return StructCompBYS();
		}
		CVec2 Structure::OriginC(void)
		{
			return m_structureOriginChunk;
		}
		WVec2 Structure::OriginW(void)
		{
			return m_structureOriginWorld;
		}
	}
}