#include "structure.h"

namespace minecraft
{
	namespace structures
	{
		Structure::Structure(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, GenRange sr, 
			WVec2 worldCoord, structure_t s)
			: m_worldChunkCoord(wcc), m_structGenRange(sr), m_structureOriginChunk(originCoordInChunk), 
			m_structureOriginWorld(worldCoord), m_structureType(s)
		{
		}
		Structure::Structure(chunk::WCoordChunk wcc, GenRange sr, WVec2 worldCoord, structure_t s)
			: m_worldChunkCoord(wcc), m_structGenRange(sr), 
			m_structureOriginWorld(worldCoord), m_structureType(s)
		{

		}
		GenRange& Structure::GRange(void)
		{
			return m_structGenRange;
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
		structure_t Structure::SType(void)
		{
			return m_structureType;
		}
	}
}