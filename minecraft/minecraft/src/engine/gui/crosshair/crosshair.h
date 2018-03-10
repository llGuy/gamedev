#ifndef CROSSHAIR_HEADER
#define CROSSHAIR_HEADER

#include "../gui.h"
#include "../../../texture/texture.h"
#include "../quad/quad.h"

namespace minecraft
{
	namespace gui
	{
		class Crosshair
			: public GUI
		{
		public:
			explicit Crosshair(const glm::vec2 p, const glm::vec2 s, TextureAtlas& ta);
			void Init(const glm::mat4& projection) override;
			void* IndexOffset(void) override;
		private:
			Quad m_quad;
		};
	}
}

#endif