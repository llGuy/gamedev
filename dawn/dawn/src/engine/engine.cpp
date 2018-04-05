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
		m_program.Init(ShaderBase{ "res\\shaders\\vsh.shader", GL_VERTEX_SHADER },ShaderBase{ "res\\shaders\\fsh.shader", GL_FRAGMENT_SHADER });
		m_program.Compile();
		m_program.Link("vertex_position", "vertex_color");
		m_program.UseProgram();
	}

	void DawnEngine::UdataInit(void)
	{
		m_program.GetUniformLocations(UDataLoc( UDType::MAT4, "projection_matrix" ), 
			UDataLoc( UDType::MAT4, "model_matrix" ), 
			UDataLoc( UDType::MAT4, "view_matrix" ));
	}

	void DawnEngine::Render(void)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float a = 0.0f;
		//m_renderer.UniformData(m_program, m_uniformData, m_uniformDataLocs)
		m_program.UniformData(&a);
		m_renderer.DrawElements(m_mesh.RenderParams(), GL_TRIANGLES);
	}

}