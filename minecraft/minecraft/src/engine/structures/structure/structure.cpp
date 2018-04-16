#include "structure.h"

namespace minecraft
{
	namespace structures
	{
		Structure::Structure(chunk::WCoordChunk wcc, CVec2 originCoordInChunk, GenRange sr, 
			WVec2 worldCoord, structure_t s, const int32_t height)
			: m_worldChunkCoord(wcc), m_structGenRange(sr), m_structureOriginChunk(originCoordInChunk), 
			m_structureOriginWorld(worldCoord), m_structureType(s), m_oheight(height)
		{
		}
		Structure::Structure(chunk::WCoordChunk wcc, GenRange sr, 
			WVec2 worldCoord, structure_t s, const int32_t height)
			: m_worldChunkCoord(wcc), m_structGenRange(sr), 
			m_structureOriginWorld(worldCoord), m_structureType(s), m_oheight(height)
		{

		}
		const uint32_t* Structure::Size(void)
		{
			return nullptr;
		}
		GenRange& Structure::GRange(void)
		{
			return m_structGenRange;
		}
		GenRange Structure::GenerateStructRangeAll(void) const
		{
			return GenRange();
		}
		int32_t Structure::Height(void) const
		{
			return m_oheight;
		}
		StructCompBYS Structure::GenerateYStripOfStruct(WVec2 worldCoord)
		{
			return StructCompBYS();
		}
		CVec2 Structure::OriginC(void) const
		{
			return m_structureOriginChunk;
		}
		WVec2 Structure::OriginW(void) const
		{
			return m_structureOriginWorld;
		}
		structure_t Structure::SType(void) const
		{
			return m_structureType;
		}
	}
}