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

			void CGPUHandler::RemoveBlock(uint16_t i)
			{
				m_deletedBlocks[m_deletedBlocksPointer] = i;
				for (uint32_t index = 0; index < m_deletedBlocksPointer; ++index)
					if (i > m_deletedBlocks[index]) --i;
				++m_deletedBlocksPointer;

				m_blocks.erase(m_blocks.begin() + i);
			}
		}
	}
}