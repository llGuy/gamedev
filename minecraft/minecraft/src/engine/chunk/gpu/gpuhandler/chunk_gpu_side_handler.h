#ifndef CHUNK_GPU_SIDE_HANDLER_HEADER
#define CHUNK_GPU_SIDE_HANDLER_HEADER

#include <array>
#include <vector>

#include "../../database/blockystrip.h"
#include "../cgpubuffer/cgpubuffer.h"
#include "../gpublockdata.h"

namespace minecraft
{
	namespace chunk
	{
		namespace gpu
		{
			/* chunk GPU side handler will create the buffer with the VAOs */
			class CGPUHandler
			{
			public:
				enum blockqualifier_t
					: uint32_t
				{
					BLOCKS = true,

					TRANSPARENT_BLOCKS = false
				};

				explicit CGPUHandler(void);
				~CGPUHandler(void);
			public:
				void PrepareVector(void);
				void DestroyVector(void);
				/* initializations are gradual							*/
				/* (done in the for loop in the ChunkDB)				*/
				void Load(BlockYStrip* ys, uint32_t index, int32_t y,
					WVec2 chunkCoords, WVec2 negCorner);
				void PushBack(const glm::vec3& position, const TextureData& t);
				::std::size_t NumBlocks(void);

				void DestroyBuffer(void);
			public:
				/* gets called in Init to initialize the texture maps	*/
				/* for the buffers to map   the blocks properly			*/
				/* in order to instance the block drawing				*/
				void LoadGPUBuffer(void);
				VAO* Vao(void);
				const bool CreatedVAO(void);
				void RemoveBlock(uint16_t& i);
				std::array<uint32_t, 16>& DeletedBlocksIndices(void);
				uint32_t& DBPointer(void);
				uint32_t MaxDBPointer(void);
			private:
				CGPUBuffer m_buff;
				std::vector<BData> m_blocks;
				uint32_t m_deletedBlocksPointer;
				std::array<uint32_t, 16> m_dBlocks;
			};
		}
	}
}

#endif
