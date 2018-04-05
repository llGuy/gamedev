#include "engine.h"

namespace dawn {

	DawnEngine::DawnEngine(const int32_t& width, const int32_t& height) noexcept
		: m_mesh(glm::vec2(0.0f))
	{
	}

	void DawnEngine::Init(void)
	{
		glEnable(GL_DEPTH_TEST);
		m_mesh.GenerateData();
		m_mesh.CreateOpenGLObjs();
		ShadersInit();
	}

	void DawnEngine::ShadersInit(void)
	{
		m_program.Init(ShaderBase{ "res\\shaders\\vsh.shader", GL_VERTEX_SHADER },
			ShaderBase{ "res\\shaders\\fsh.shader", GL_FRAGMENT_SHADER });
		m_program.Compile();
		m_program.Link("vertex_position", "vertex_color");
		m_program.UseProgram();
	}

	void DawnEngine::UdataInit(void)
	{
		m_uniformDataLocs.projectionLoc = m_program.GetUniformLocation("projection_matrix");
		m_uniformDataLocs.modelLoc = m_program.GetUniformLocation("model_matrix");
		m_uniformDataLocs.viewLoc = m_program.GetUniformLocation("view_matrix");
	}

	void DawnEngine::Render(void)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_renderer.DrawElements(m_mesh.RenderParams(), GL_TRIANGLES);
	}

}