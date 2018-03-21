#include "cmap.h"

namespace minecraft
{
	namespace chunk
	{
		namespace cmap
		{
			CMap::CMap(int32_t seed)
				: m_nll(4), m_size(0), m_seed(seed), m_llists(new std::vector<std::list<Chunk>>), m_updateState(update_t::UPDATE_INACTIVE)
			{
				for (uint32_t i = 0; i < m_nll; ++i)
					m_llists->push_back(std::list<Chunk>());
			}
			void CMap::Load(Chunk::WCoordChunk& v)
			{
				int32_t h = CHash()(v) % m_llists->size();
				for (auto& i : m_llists->operator[](h))
					if (i.ChunkCoordinate() == v) return;
				App(v, m_seed);
			}
			Chunk& CMap::operator[](Chunk::WCoordChunk& v)
			{
				int32_t h = CHash()(v) % m_llists->size();
				auto& l = m_llists->operator[](h);
				for (auto& i : l)
					if (i.ChunkCoordinate() == v)
						return i;
				return App(v, m_seed);
			}
			//Chunk& CMap::At(Chunk::WCoordChunk& v, int32_t seed)
			//{
			//	int32_t h = CHash()(v) % m_llists->size();
			//	auto& l = m_llists->operator[](h);
			//	for (auto& i : l)
			//		if (i.ChunkCoordinate() == v)
			//			return i;
			//	return App(v, seed);
			//}
			const bool CMap::Exists(Chunk::WCoordChunk& v)
			{
				int32_t h = CHash()(v) % m_llists->size();
				for (auto& i : m_llists->operator[](h))
					if (i.ChunkCoordinate() == v)
						return i.Loaded();
				return false;
			}
			void CMap::AfterGLEWInit(void)
			{
				for (auto& i : *m_llists)
					for (auto& j : i)
					{
						j.LoadGPUBuffer();
					}
			}
			CMap::iterator CMap::Begin(void)
			{
				return m_llists->begin();
			}
			CMap::iterator CMap::End(void)
			{
				return m_llists->end();
			}
			::std::size_t CMap::Size(void) const
			{
				return m_size;
			}
			CMap::update_t CMap::UpdateState(void)
			{
				return m_updateState;
			}
			const bool CMap::DeletedLLists(void)
			{
				return m_deletedCurrentLLists;
			}
			void CMap::ResetDeltedLListsBool(void)
			{
				m_deletedCurrentLLists = false;
			}
			/* append */
			Chunk & CMap::App(const Chunk::WCoordChunk& v, int32_t seed)
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
							int32_t h = CHash()(wcc) % newl->size();
							newl->operator[](h).push_back(j);
						}
					}
					std::vector<std::list<Chunk>>* current = m_llists;
					m_llists = newl;
					m_updateState = update_t::UPDATE_INACTIVE;
					m_deletedCurrentLLists = true;
					delete current;
				}
				int32_t h = CHash()(v) % m_llists->size();
				m_llists->operator[](h).push_back(Chunk(v, seed));
				Chunk& newChunk = m_llists->operator[](h).back();
				++m_size;
				return newChunk;
			}
			void CMap::Erase(Chunk::WCoordChunk& v)
			{
				int32_t h = CHash()(v) % m_llists->size();
				std::list<Chunk> l = m_llists->operator[](h);
				/*for (auto& i : l)
					if (i.ChunkCoordinate() == v)
					{
						m_llists->operator[](h).erase(l.begin());
					}*/
				for (auto c = l.begin(); c != l.end();)
				{
					if (c->ChunkCoordinate() == v)
					{
						c->DestroyGPUBuffer();
						l.erase(c);
						return;
					}
					else ++c;
				}
			}
		}
	}
}