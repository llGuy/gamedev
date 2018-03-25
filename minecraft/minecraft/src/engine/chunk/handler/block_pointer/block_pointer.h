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
			void RecieveBlockPosition(const glm::vec3& pos, CVec2 bc, WVec2 chunkCoord);
			const glm::vec3& BlockPosition(void);
			const bool PointingAtBlock(void);
			void* Offset(void);
			const uint32_t& BufferID(void);
			VAO* Vao(void);
			const glm::mat4& TranslateMatrix(void);

			const CVec2& BlockCoord(void);
			const WVec2& ChunkCoord(void);
		private:
			bool m_pointingAtBlock;
			glm::vec3 m_translation;
			glm::mat4 m_translateMatrix;
			BlockOutlineGPUBuffer m_gpubuffer;

			CVec2 m_blockCoordInChunk;
			WVec2 m_chunkCoord;
		};
	}
}

#endif