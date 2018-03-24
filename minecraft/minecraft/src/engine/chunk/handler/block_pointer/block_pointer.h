#ifndef BLOCK_POINTER_HEADER
#define BLOCK_POINTER_HEADER

#include "block_outline/gpubuffer/block_outline_buffer.h"

namespace minecraft
{
	namespace chunk
	{
		class BlockPointer
		{
		public:
			explicit BlockPointer(void) 
				: m_pointingAtBlock(false)
			{
			}
			void NoBlocks(void);
			void CreateBuffer(void);
			void RecieveBlockPosition(const glm::vec3& pos);
			const glm::vec3& BlockPosition(void);
			const bool PointingAtBlock(void);
		private:
			bool m_pointingAtBlock;
			glm::vec3 m_blockPosition;
			BlockOutlineGPUBuffer m_gpubuffer;
		};
	}
}

#endif