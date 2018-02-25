#include "chunk_gpu_side_handler.h"
#include "../../database/chunk_data_base.h"

namespace minecraft
{
	namespace chunk
	{
		namespace gpu
		{
			CGPUHandler::CGPUHandler(void)
				: m_buff(), m_deletedBlocksPointer(0)
			{
			}

			void CGPUHandler::PrepareVector(void)
			{
			}
			
			void CGPUHandler::DestroyVector(void)
			{
			}

			void CGPUHandler::Load(BlockYStrip* ys,
				uint32_t index, int32_t y, WVec2 chunkCoords, WVec2 negCorner)
			{
				Block* b = &(ys->ystrip[y]);
				b->VIndex() = static_cast<uint16_t>(m_blocks.size());

				glm::vec3 position = b->WPos(chunkCoords, y, negCorner);
				m_blocks.push_back({ position, b->TextureD() });
			}

			void CGPUHandler::PushBack(glm::vec3& position, const TextureData& t)
			{
				m_blocks.push_back({ position, t });
			}

			::std::size_t CGPUHandler::NumBlocks(void)
			{
				return m_blocks.size();
			}

			void CGPUHandler::LoadGPUBuffer(void)
			{
				::std::size_t components = m_blocks.size();
				BData* p = &(m_blocks.at(0));
				m_buff.Load(components, p);
			}

			VAO* CGPUHandler::Vao(void)
			{
				return m_buff.Vao();
			}

			const bool CGPUHandler::CreatedVAO(void)
			{
				return m_buff.CreatedVAO();
			}

			void CGPUHandler::RemoveBlock(uint16_t& i)
			{
				uint16_t original = i;
				m_dBlocks[m_deletedBlocksPointer] = i;
				for (uint32_t index = 0; index < m_deletedBlocksPointer; ++index)
					if (original > m_dBlocks[index]) --i;
				++m_deletedBlocksPointer;

				m_blocks.erase(m_blocks.begin() + i);
			}

			std::array<uint32_t, 16>& CGPUHandler::DeletedBlocksIndices(void)
			{
				return m_dBlocks;
			}

			uint32_t& CGPUHandler::DBPointer(void)
			{
				return m_deletedBlocksPointer;
			}

			uint32_t CGPUHandler::MaxDBPointer(void)
			{
				return 16;
			}
		}
	}
}