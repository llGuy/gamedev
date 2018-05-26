#include "engine.h"
#include "configs/configs.h"

namespace dawn {

    DawnEngine::DawnEngine(int32_t width, int32_t height) 
	: m_player(new ent::Player(configs::DEFAULT_PLAYER_POSITION, configs::DEFAULT_PLAYER_DIRECTION)), 
	  m_projectionMatrix(glm::perspective(glm::radians(configs::FOV_DEGREES), (float)width / height, 0.1f, configs::RENDER_DISTANCE)),
	  m_chandler(0, m_player)
    {
    }
    
    void DawnEngine::Init(const glm::vec2& cursorPos)
    {
	glEnable(GL_DEPTH_TEST);
	ShadersInit();
	CameraInit(cursorPos);
	TerrainInit();
    }

    void DawnEngine::RecieveAction(action_t a)
    {
	static constexpr float TEST_TIME = 10.0f;

	switch (a)
	{
	    // all types of player movement
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
	m_camera.Look(pos, configs::MOUSE_SENSITIVITY);
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

    void DawnEngine::CameraInit(const glm::vec2& cursor)
    {
	m_camera.CursorPosition(cursor);
	m_camera.Bind(m_player);
    }

    void DawnEngine::TerrainInit(void)
    {
	m_chandler.LoadChunks(8, 8);
    }

    void DawnEngine::Render(void)
    {
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewMatrix = m_camera.ViewMatrix();

	m_program.UniformData(&m_projectionMatrix[0][0], &viewMatrix[0][0]);

	for (auto chunk = m_chandler.MapBegin(); chunk != m_chandler.MapEnd(); ++chunk)
	{
	    m_renderer.DrawElements(chunk->second.RenderParams(), GL_LINE_STRIP);
	}
    }

}
