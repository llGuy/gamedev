#ifndef TREE_HEADER
#define TREE_HEADER

#include "../../chunk/cdata/cvec2.h"
#include "../../block/block.h"

namespace minecraft
{
	namespace tree
	{
		enum class tree_t
			: unsigned int
		{
			OAK = 4,		// the value is the minimum tree size

			INV
		};

		// height of the trunc
		enum class tsize_t
			: unsigned int
		{
			SMALL,

			MEDIUM,

			BIG
		};

		struct TreeComp
		{
			WVec2 xz;
			signed int y;
			Block::block_t type;
		};

		class Tree
		{
		public:
			explicit Tree(tsize_t treeSize, tree_t treeType, WVec2 position);
		public:
			void GenerateSimpleTree(void);
			const std::vector<TreeComp>& TreeBlocks(void);
		protected:
			tsize_t m_treeSize;
			tree_t m_treeType;
			WVec2 m_position;
			std::vector<TreeComp> m_treeBlocks;
		};
	}
}

#endif