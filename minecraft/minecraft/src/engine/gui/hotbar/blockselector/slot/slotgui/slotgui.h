#ifndef SLOTGUI_HEADER
#define SLOTGUI_HEADER

#include "../../../../../block/block.h"
#include "../../../../gui.h"
#include "../../../../../texture_atlas/texture_atlas.h"
#include <glm/gtx/transform.hpp>

// TESTING

namespace minecraft
{
	namespace gui
	{
		class SlotGUI
			: public GUI
		{
		public:
			explicit SlotGUI(const float& stride, const glm::vec2&,
				const glm::vec2&, TextureAtlas* t);
		private:
			void CreateBlockImage(void);
		private:
			Block::block_t m_btype;
			Quad m_faces[3];
		};
	}
}

#endif