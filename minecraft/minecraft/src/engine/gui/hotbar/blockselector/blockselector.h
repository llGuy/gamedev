#ifndef BLOCKSELECTOR_HEADER
#define BLOCKSELECTOR_HEADER

#include "slot/slot.h"
#include "../../gui.h"
#include "../hotbarposition.h"
#include "slot/slotgui/slotgui.h"
#include "../../../renderer/glderenderdata/glderenderdata.h"

namespace minecraft
{
	namespace gui
	{
		class BlockSelector
			: public GUI
		{
		public:
			explicit BlockSelector(const position_t& p, const float& stride, const glm::vec2&,
				const float, TextureAtlas &t, TextureAtlas* blockt);
			void Update(const int32_t& slot) override;
			void Init(const glm::mat4& projection) override;
			void* IndexOffset(void) override;
			const Block::block_t& SelectedBlock(void) override;
			rnd::GLDrawElementsRenderDataAll DrawData(void) override;
		private:
			void RenderDataInit(void);
			void BlockSelectorRndDataInit(void);
			void SlotGUIRndDataInit(void);
		private:
			float m_stride;
			position_t m_position;
			int32_t m_slot;
			Quad m_selector;
			Slot m_slots[9];
			rnd::GLDrawElementsRenderData m_rndData[10];
		};
	}
}

#endif