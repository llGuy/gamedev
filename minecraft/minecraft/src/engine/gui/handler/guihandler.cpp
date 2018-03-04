#include "guihandler.h"

namespace minecraft
{
	namespace gui
	{
		GUIHandler::GUIHandler(void)
		{
		}
		void GUIHandler::Init(const glm::mat4& projection)
		{
			m_shp.Init("res\\shaders\\ui\\vsh.shader", "res\\shaders\\ui\\fsh.shader", "INV");
			m_shp.Compile();
			m_shp.Link({"vertex_position", "texture_data"});
			// crosshair
			m_guis[CROSSHAIR] = new Crosshair(glm::vec2(0.0f), glm::vec2(0.01f));
			m_guis[CROSSHAIR]->Init(projection);
		}
		void GUIHandler::UseProgram(void)
		{
			m_shp.UseProgram();
		}
		const uint32_t GUIHandler::Size(void) const
		{
			return SIZE;
		}
		rnd::GLDrawElementsRenderData GUIHandler::RenderData(const uint32_t& index)
		{
			rnd::GLDrawElementsRenderData d;
			d.bufferData = m_guis[index]->BufferID();
			d.offsetIndices = (void*)sizeof(Quad);
			d.vao = m_guis[index]->Vao();
			d.t = m_guis[index]->Tex();
			return d;
		}
	}
}