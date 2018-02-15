#include "tree.h"

namespace minecraft
{
	namespace tree
	{
		Tree::Tree(tsize_t size, tree_t type, WVec2 position)	
			: m_treeSize(size), m_treeType(type), m_position(position)
		{
		}
		void Tree::GenerateSimpleTree(void)
		{
			// generate the blocks for the tree
		}
		const std::vector<TreeComp>& Tree::TreeBlocks(void)
		{
			return m_treeBlocks;
		}
	}
}