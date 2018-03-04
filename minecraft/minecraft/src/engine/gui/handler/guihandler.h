#ifndef GUIHANDLER_HEADER
#define GUIHANDLER_HEADER

#include "../crosshair/crosshair.h"
#include "../../../shader/shprogram.h"
#include "../../renderer/glderenderdata/glderenderdata.h"

namespace minecraft
{
	namespace gui
	{
		class GUIHandler
		{
		public:
			explicit GUIHandler(void);
			void Init(const glm::mat4& projection);

			const uint32_t Size(void) const;
			rnd::GLDrawElementsRenderData RenderData(const uint32_t& index);
			void UseProgram(void);
		private:
			enum
			{
				CROSSHAIR
			};
			const uint32_t SIZE = 1;
			GUI* m_guis[1];
			sh::SHProgram m_shp;
		};
	}
}

#endif