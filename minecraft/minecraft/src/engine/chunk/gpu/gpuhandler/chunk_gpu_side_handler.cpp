#include "chunk_gpu_side_handler.h"
#include "../../database/chunk_data_base.h"

namespace minecraft
{
	namespace chunk
	{
		namespace gpu
		{
			CGPUHandler::CGPUHandler(void)
				: m_buff()
			{
			}

			void CGPUHandler::PrepareVector(void)
			{
				m_blocks = new std::vector<BData>;
			}
			
			void CGPUHandler::DestroyVector(void)
			{
				delete m_blocks;
			}

			void CGPUHandler::Load(BlockYStrip* ys,
				unsigned int index, signed int y, WVec2 chunkCoords, WVec2 negCorner)
			{
				Block* b = &(ys->ystrip[y]);
				glm::vec3 position = b->WPos(chunkCoords, y, negCorner);
				m_blocks->push_back({ position, b->TextureD() });
			}

			::std::size_t CGPUHandler::NumBlocks(void)
			{
				return m_blocks->size();
			}

			void CGPUHandler::LoadGPUBuffer(void)
			{
				::std::size_t components = m_blocks->size();
				BData* p = &(m_blocks->at(0));
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
		}
	}
}