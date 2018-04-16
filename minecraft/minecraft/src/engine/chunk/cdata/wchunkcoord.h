#ifndef WCHUNKCOORD_HEADER
#define WCHUNKCOORD_HEADER

#include "cvec2.h"

namespace minecraft
{
	namespace chunk
	{
		struct WCoordChunk
		{
			WCoordChunk(void) = default;
			WCoordChunk(const WVec2& v)
				: wpos(v) {}
			WVec2 wpos;
			bool operator==(const WCoordChunk& c)
			{
				return (c.wpos.x == wpos.x && c.wpos.z == wpos.z);
			}
			bool operator!=(const WCoordChunk& c)
			{
				return (c.wpos.x != wpos.x || c.wpos.z != wpos.z);
			}
		};
	}
}

#endif