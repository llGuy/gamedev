#include "engine.h"

namespace dawn {

	DawnEngine::DawnEngine(const int32_t& width, const int32_t& height) 
		: m_mesh(glm::vec2(0.0f)), 
		m_player(new ent::Player(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, -0.25f, 1.0f))), 
		m_camera(),
		m_projectionMatrix(glm::perspective(glm::radians(70.0f), (float)width / height, 0.1f, 100.0f))
	{
	}

	void DawnEngine::Init(const glm::vec2& cursorPos)
	{
		glEnable(GL_DEPTH_TEST);
		m_mesh.GenerateData();
		m_mesh.CreateOpenGLObjs();
		ShadersInit();
		m_camera.CursorPosition(cursorPos);
		m_camera.Bind(m_player);
	}

	void DawnEngine::RecieveAction(const action_t& a)
	{
		static constexpr float TEST_TIME = 10.0f;

		switch (a)
		{
		case action_t::W: m_player->Move(ent::movement_t::FORWARD, TEST_TIME); return;
		case action_t::A: m_player->Strafe(ent::strafe_t::LEFT, TEST_TIME); return;
		case action_t::S: m_player->Move(ent::movement_t::BACKWARD, TEST_TIME); return;
		case action_t::D: m_player->Strafe(ent::strafe_t::RIGHT, TEST_TIME); return;
		case action_t::SPACE: m_player->VMove(ent::vmovement_t::UP, TEST_TIME); return;
		case action_t::LSHIFT: m_player->VMove(ent::vmovement_t::DOWN, TEST_TIME); return;
		}
	}

	void DawnEngine::MouseCursor(const glm::vec2& pos)
	{
		static constexpr float TEST_SENSITIVITY = 0.02f;
		m_camera.Look(pos, TEST_SENSITIVITY);
	}

	void DawnEngine::ShadersInit(void)
	{
		m_program.Init(ShaderBase{ "res\\shaders\\mesh\\vsh.shader", GL_VERTEX_SHADER },
			ShaderBase{ "res\\shaders\\mesh\\fsh.shader", GL_FRAGMENT_SHADER });
		m_program.Compile();
		m_program.Link("vertex_position", "vertex_color");
		m_program.UseProgram();
		UdataInit();
	}

	void DawnEngine::UdataInit(void)
	{
		m_program.GetUniformLocations(UDataLoc( UDType::MAT4, "projection_matrix" ), 
			UDataLoc( UDType::MAT4, "view_matrix" ));
	}

	void DawnEngine::Render(void)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 viewMatrix = m_camera.ViewMatrix();

		m_program.UniformData(&m_projectionMatrix[0][0], &viewMatrix[0][0]);
		m_renderer.DrawElements(m_mesh.RenderParams(), GL_TRIANGLES);
	}

}