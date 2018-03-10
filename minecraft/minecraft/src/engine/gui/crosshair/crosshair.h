#ifndef CROSSHAIR_HEADER
#define CROSSHAIR_HEADER

#include "../gui.h"
#include "../../../texture/texture.h"
#include "../quad/quad.h"
#include "../../renderer/glderenderdata/glderenderdata.h"

namespace minecraft
{
	namespace gui
	{
		class Crosshair
			: public GUI
		{
		public:
			explicit Crosshair(const glm::vec2 p, const float s, TextureAtlas& ta);
			void Init(const glm::mat4& projection) override;
			void* IndexOffset(void) override;
			rnd::GLDrawElementsRenderDataAll DrawData(void) override;
		private:
			void RenderDataInit(void);
		private:
			Quad m_quad;
			rnd::GLDrawElementsRenderData m_rndData[1];
		};
	}
}

#endif