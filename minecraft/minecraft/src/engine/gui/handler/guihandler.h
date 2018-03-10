#ifndef GUIHANDLER_HEADER
#define GUIHANDLER_HEADER

#include "../crosshair/crosshair.h"
#include "../../../shader/shprogram.h"
#include "../hotbar/hotbar.h"
#include "../guitype.h"
#include "../../texture_atlas/texture_atlas.h"
#include "../guievent_handler/guieventhandler.h"
#include "../../renderer/glderenderdata/glderenderdata.h"

namespace minecraft
{
	namespace gui
	{
		class GUIHandler
		{
		public:
			explicit GUIHandler(TextureAtlas* ptr);
			~GUIHandler(void);
			void Init(const glm::mat4& projection);

			const uint32_t Size(void) const;
			rnd::GLDrawElementsRenderData RenderData(const uint32_t& index);
			void UseProgram(void);
			const Block::block_t& HotbarSelectedBlock(void);
			void Event(const GUIEventHandler::event_t& e, int32_t slot = 0);
		private:
			const uint32_t SIZE = 3;
			GUI* m_guis[3];
			TextureAtlas* m_blockTextureAtlasPtr;
			TextureAtlas m_guiTextureAtlas;
			TextureAtlas m_crosshair;

			sh::SHProgram m_shp;
			GUIEventHandler m_guieventHandler;
		};
	}
}

#endif