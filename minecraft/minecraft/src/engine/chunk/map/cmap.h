#ifndef CMAP_HEADER
#define CMAP_HEADER

#include <list>
#include <unordered_map>
#include <vector>

#include "../chunk.h"
#include "mutex/render_update_mutex.h"
//#include "../loader/chunk_loader.h"

namespace minecraft
{
	namespace chunk
	{
		namespace cmap
		{
			struct CHash
			{
				int operator()(const Chunk::WCoordChunk& v) const
				{
					return std::hash<int32_t>()(v.wpos.x)
						^ (std::hash<int32_t>()(v.wpos.z) >> 1) << 1;
				}
			};

			class CMap
			{
			public:
				using iterator = std::vector<std::list<Chunk>>::iterator;
				enum class update_t 
					: bool
				{
					UPDATE_ACTIVE = false,

					UPDATE_INACTIVE = true
				};

				CMap(void) = default;
				CMap(int32_t seed);
				void Load(Chunk::WCoordChunk& v);
				Chunk& operator[](Chunk::WCoordChunk& v);
				const bool Exists(Chunk::WCoordChunk& v);
				void Erase(Chunk::WCoordChunk& v);
				void AfterGLEWInit(void);
				iterator Begin(void);
				iterator End(void);
				::std::size_t Size(void) const;
				update_t UpdateState(void);
				const bool DeletedLLists(void);
				void ResetDeltedLListsBool(void);
			private:
				/* append */
				Chunk & App(const Chunk::WCoordChunk& v, int32_t seed);
			private:
				std::vector<std::list<Chunk>>* m_llists;
				/* number of linked lists */
				std::size_t m_nll;
				/* size of the Chunk Map */
				std::size_t m_size;
				int32_t m_seed;
				
				bool m_deletedCurrentLLists;
				update_t m_updateState;
			};
		}
	}
}

#endif