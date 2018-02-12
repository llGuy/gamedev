#include "engine.h"

#include <glm/gtx/transform.hpp>
#include <thread>

namespace minecraft
{
	Engine::Engine(void)
		: m_camera(), m_textureAtlas("res\\textures\\texture_atlas.png")
	{
		Init();
	}
	void Engine::Render(void)
	{
		glClearColor(m_udata.skyColor.r, m_udata.skyColor.g, m_udata.skyColor.b, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateData();
		for (auto it = m_chunkHandler->Begin(); it != m_chunkHandler->End(); ++it)
		{
			for (auto& jt : *it)
			{
				if (m_chunkHandler->MapDeltedLLists())
				{
					m_chunkHandler->ResetMapDeletedLListsBool();
					return;
				}
				chunk::Chunk& c = jt;
				if (c.Loaded() && !c.BufferLoaded())
				{
					c.LoadGPUBuffer();
				}
				if (c.Loaded() && c.BufferLoaded() && c.Vao() != nullptr && c.CreatedVAO())
				{
					m_renderer.UniformData(m_udata, m_chunkHandler->Locations());
					m_renderer.AInstancedRender(GL_POINTS, c.Vao(), 0, 1, c.NumBlocks());
				}
			}
		}
	}
	void Engine::UpdateData(void)
	{
		UpdatePlayerData();
		UpdateUniformData();
	}
	void Engine::Init(void)
	{
		m_player = new ent::Player();
		m_chunkHandler = new chunk::CHandler(static_cast<signed int>(time(NULL)));
		m_chunkHandler->Init();
		Configure();
	}
	void Engine::TimeDataInit(void)
	{
		m_time.currentTime = std::chrono::high_resolution_clock::now();
		m_time.beginning = std::chrono::high_resolution_clock::now();
	}
	void Engine::Configure(void)
	{
		m_variableConfigs.FOV = glm::radians(60.0f);
		m_variableConfigs.renderDistance = 50.0f;
		m_variableConfigs.mouseSensitivity = 0.02f;

		m_constantConfigs.gravity = glm::vec3(0.0f, -8.6f, 0.0f);
	}
	void Engine::UDataInit(unsigned int wwidth, unsigned int wheight)
	{
		m_chunkHandler->GetUniform();

		m_udata.projectionMatrix = glm::perspective(m_variableConfigs.FOV, (float)wwidth / wheight, 0.1f, 300.0f);
		m_udata.lightPosition = glm::vec3(0.0f, 100.0f, 0.0f);
	}
	void Engine::AfterGLEWInit(unsigned int wwidth, unsigned int wheight, 
		glm::vec2 cursorPosition, GLFWwindow* window)
	{
		m_textureAtlas.Init();
		m_textureAtlas.Bind(0);
		m_chunkHandler->AfterGLEWInit();
		m_chunkHandler->LaunchChunkLoader(m_player, window);
		UDataInit(wwidth, wheight);
		TimeDataInit();
		m_camera = ent::Camera(cursorPosition);
		m_camera.Bind(m_player);

		m_chunkHandler->UseSHProgram();
	}
	void Engine::RecieveKeyInput(key_t&& key)
	{
		switch (key)
		{
		case key_t::W:
			m_player->Move(ent::Entity::move_t::FORWARD, &m_time);
			break;
		case key_t::A:
			m_player->Strafe(ent::Entity::strafe_t::LEFT, &m_time);
			break;
		case key_t::S:
			m_player->Move(ent::Entity::move_t::BACKWARD, &m_time);
			break;
		case key_t::D:
			m_player->Strafe(ent::Entity::strafe_t::RIGHT, &m_time);
			break;
		case key_t::SPACE:
			m_player->Move(ent::Entity::move_t::JUMP, &m_time);
			break;
		case key_t::LSHIFT:
			m_player->VMove(ent::Entity::vmove_t::DOWN, &m_time);
			break;
		}
	}
	void Engine::RecieveMouseMovement(glm::vec2 newMousePosition)
	{
		m_camera.Look(newMousePosition, m_variableConfigs.mouseSensitivity);
	}
	void Engine::UpdateUniformData(void)
	{
		m_udata.viewMatrix = m_camera.ViewMatrix();
		m_udata.eyePosition = *(m_player->EntityWorldPosition());
		m_udata.skyColor = glm::vec3(0.2f, 0.4f, 1.0f);

		m_time.deltaT = (double)((std::chrono::high_resolution_clock::now() - m_time.currentTime).count()) / 1000000000;
		m_time.currentTime = std::chrono::high_resolution_clock::now();
	}
	void Engine::UpdatePlayerData(void)
	{
		float blockUnderPlayer = m_chunkHandler->HighestBlock(*m_player->EntityWorldPosition());
		m_player->UpdData(&m_constantConfigs.gravity, blockUnderPlayer, m_time.deltaT);
	}
}