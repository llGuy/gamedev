#ifndef CMAP_HEADER
#define CMAP_HEADER

#include <list>
#include <unordered_map>
#include <vector>

#include "../chunk.h"
#include "../loader/chunk_loader.h"

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
					return std::hash<int>()(v.wpos.x)
						^ (std::hash<int>()(v.wpos.z) >> 1) << 1;
				}
			};

			class CMap
			{
			public:
				using iterator = std::vector<std::list<Chunk>>::iterator;
				enum class update_t : bool
				{
					UPDATE_ACTIVE = false,

					UPDATE_INACTIVE = true
				};

				CMap(void) = default;
				CMap(signed int seed)
					: m_nll(4), m_size(0), m_seed(seed), m_llists(new std::vector<std::list<Chunk>>), m_updateState(update_t::UPDATE_INACTIVE)
				{
					for (unsigned int i = 0; i < m_nll; ++i)
						m_llists->push_back(std::list<Chunk>());
				}
				void Load(Chunk::WCoordChunk& v)
				{
					int h = CHash()(v) % m_llists->size();
					for (auto& i : m_llists->operator[](h))
						if (i.ChunkCoordinate() == v) return;
					App(v);
				}
				Chunk& operator[](Chunk::WCoordChunk& v)
				{
					int h = CHash()(v) % m_llists->size();
					for (auto& i : m_llists->operator[](h))
						if (i.ChunkCoordinate() == v) 
							return i;
					return App(v);
				}
				void AfterGLEWInit(void)
				{
					for (auto& i : *m_llists)
						for (auto& j : i)
						{
							j.LoadGPUBuffer();
						}
				}
				iterator Begin(void)
				{
					return m_llists->begin();
				}
				iterator End(void)
				{
					return m_llists->end();
				}
				::std::size_t Size(void) const
				{
					return m_size;
				}
				update_t UpdateState(void)
				{
					return m_updateState;
				}
				const bool DeletedLLists(void)
				{
					return m_deletedCurrentLLists;
				}
				void ResetDeltedLListsBool(void)
				{
					m_deletedCurrentLLists = false;
				}
			private:
				/* append */
				Chunk & App(const Chunk::WCoordChunk& v)
				{
					if (m_size == m_nll)
					{
						std::vector<std::list<Chunk>>* newl = new std::vector<std::list<Chunk>>;
						m_updateState = update_t::UPDATE_ACTIVE;
						m_nll *= 2;
						newl->resize(m_nll);
						for (auto& i : *m_llists)
						{
							for (auto& j : i)
							{
								Chunk::WCoordChunk wcc = j.ChunkCoordinate();
								int h = CHash()(wcc) % newl->size();
								newl->operator[](h).push_back(j);
							}
						}
						std::vector<std::list<Chunk>>* current = m_llists;
						m_llists = newl;
						m_updateState = update_t::UPDATE_INACTIVE;
						m_deletedCurrentLLists = true;
						delete current;
					}
					int h = CHash()(v) % m_llists->size();
					m_llists->operator[](h).push_back(Chunk(m_seed));
					Chunk& newChunk = m_llists->operator[](h).back();
					++m_size;
					return newChunk;
				}
			private:
				std::vector<std::list<Chunk>>* m_llists;
				/* number of linked lists */
				std::size_t m_nll;
				/* size of the Chunk Map */
				std::size_t m_size;
				signed int m_seed;
				
				bool m_deletedCurrentLLists;
				update_t m_updateState;
			};
		}
	}
}

#endif