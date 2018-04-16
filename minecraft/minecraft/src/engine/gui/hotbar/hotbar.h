#ifndef HOTBAR_HEADER
#define HOTBAR_HEADER

#include "../gui.h"
#include "hotbarposition.h"
#include "blockselector/blockselector.h"

namespace minecraft
{
	namespace gui
	{
		class Hotbar
			: public GUI
		{
		public:
			explicit Hotbar(const position_t& p, TextureAtlas& ta);
			void Init(const glm::mat4& projection) override;
			void* IndexOffset(void) override;
			const glm::vec2& Position(void) override;
			const float Stride(void) override;
			rnd::GLDrawElementsRenderDataAll DrawData(void) override;
		private:
			void RenderDataInit(void);
		private:
			Quad m_hotbar;
			GUI* m_blockSelector;
			rnd::GLDrawElementsRenderData m_rndData[1];
			position_t m_position;
			float m_stride;
		};
	}
}

#endif