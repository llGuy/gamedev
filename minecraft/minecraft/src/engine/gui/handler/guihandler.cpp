#include "guihandler.h"

namespace minecraft
{
	namespace gui
	{
		GUIHandler::GUIHandler(TextureAtlas* ptr)
			: m_blockTextureAtlasPtr(ptr), m_crosshair(1.0f, 1.0f, "res\\textures\\gui\\crosshair.png"),
			m_guiTextureAtlas(14.0f, 14.0f, "res\\textures\\gui\\menu\\menu_atlas.png")
		{
		}
		GUIHandler::~GUIHandler(void)
		{
			for (uint32_t gui = 0; gui < SIZE; ++gui)
			{
				delete m_guis[gui];
			}
		}
		void GUIHandler::Init(const glm::mat4& projection)
		{
			m_shp.Init("res\\shaders\\ui\\vsh.shader", "res\\shaders\\ui\\fsh.shader", "INV");
			m_shp.Compile();
			m_shp.Link({"vertex_position", "texture_data"});

			m_crosshair.Init();
			m_guiTextureAtlas.Init();

			// crosshair
			m_guis[CROSSHAIR] = new Crosshair(glm::vec2(0.0f), 0.02f, m_crosshair);
			m_guis[CROSSHAIR]->Init(projection);

			m_guis[HOTBAR] = new Hotbar(position_t::BOTTOM, m_guiTextureAtlas);
			m_guis[HOTBAR]->Init(projection);

			m_guis[BLOCKSELECTOR] = new BlockSelector(position_t::BOTTOM, 
				m_guis[HOTBAR]->Stride(), m_guis[HOTBAR]->Position(), 1.0f, *(m_guis[HOTBAR]->Tex()), m_blockTextureAtlasPtr);
			m_guis[BLOCKSELECTOR]->Init(projection);
		}
		void GUIHandler::UseProgram(void)
		{
			m_shp.UseProgram();
		}
		const uint32_t GUIHandler::Size(void) const
		{
			return SIZE;
		}
		void GUIHandler::Event(const GUIEventHandler::event_t& e, int32_t slot)
		{
			m_guieventHandler.HandlerEvent(e, m_guis, slot);
		}
		rnd::GLDrawElementsRenderDataAll GUIHandler::RenderData(const uint32_t& index)
		{
			return m_guis[index]->DrawData();
		}
		const Block::block_t& GUIHandler::HotbarSelectedBlock(void)
		{
			return m_guis[BLOCKSELECTOR]->SelectedBlock();
		}
	}
}