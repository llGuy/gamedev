#include "block_pointer.h"
#include <glm/gtx/transform.hpp>

namespace minecraft
{
	namespace chunk
	{
		void BlockPointer::NoBlocks(void)
		{
			m_pointingAtBlock = false;
		}
		void BlockPointer::RecieveBlockPosition(const glm::vec3& pos, CVec2 bc, WVec2 chunkCoord)
		{
			m_blockCoordInChunk = bc;
			m_chunkCoord = chunkCoord;
			m_translation = pos;
			m_translateMatrix = glm::translate(m_translation);
			m_pointingAtBlock = true;
		}
		void BlockPointer::CreateBuffer(void)
		{
			m_gpubuffer.CreateBuffer();
		}
		const glm::vec3& BlockPointer::BlockPosition(void)
		{
			return m_translation;
		}
		const bool BlockPointer::PointingAtBlock(void)
		{
			return m_pointingAtBlock;
		}
		void* BlockPointer::Offset(void)
		{
			return m_gpubuffer.Offset();
		}
		const uint32_t& BlockPointer::BufferID(void)
		{
			return m_gpubuffer.BufferID();
		}
		VAO* BlockPointer::Vao(void)
		{
			return m_gpubuffer.Vao();
		}
		const glm::mat4& BlockPointer::TranslateMatrix(void)
		{
			return m_translateMatrix;
		}
		const CVec2& BlockPointer::BlockCoord(void)
		{
			return m_blockCoordInChunk;
		}
		const WVec2& BlockPointer::ChunkCoord(void)
		{
			return m_chunkCoord;
		}
	}
}