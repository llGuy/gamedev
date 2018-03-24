#include "block_pointer.h"

namespace minecraft
{
	namespace chunk
	{
		void BlockPointer::NoBlocks(void)
		{
			m_pointingAtBlock = false;
		}
		void BlockPointer::RecieveBlockPosition(const glm::vec3& pos)
		{
			m_blockPosition = pos;
			m_gpubuffer.LoadBuffer(m_blockPosition);
		}
		void BlockPointer::CreateBuffer(void)
		{
			m_gpubuffer.CreateBuffer();
		}
		const glm::vec3& BlockPointer::BlockPosition(void)
		{
			return m_blockPosition;
		}
		const bool BlockPointer::PointingAtBlock(void)
		{
			return m_pointingAtBlock;
		}
	}
}