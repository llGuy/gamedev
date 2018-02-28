#include "engine.h"

#include <glm/gtx/transform.hpp>
#include <thread>

#define DEBUG 

namespace minecraft
{
	Engine::Engine(void)
		: m_camera(), m_textureAtlas("res\\textures\\texture_atlas.png")
	{
		Init();
	}
	Engine::~Engine(void)
	{
		
	}
	void Engine::HEAPDelete(void)
	{
		for (auto it = m_chunkHandler->Begin(); it != m_chunkHandler->End(); ++it)
			for (auto& jt : *it)
				jt.DestroyHEAPMemoryForBlocksWPos();
	}
	void Engine::Render(void)
	{
		glClearColor(m_udata.skyColor.r, m_udata.skyColor.g, m_udata.skyColor.b, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateData();
		RenderChunks();
	}
	void Engine::RenderChunks(void)
	{
		for (auto it = m_chunkHandler->Begin(); it != m_chunkHandler->End(); ++it)
		{
			for (auto& jt : *it)
			{
				// check if the chunk map deleted the lists to create new ones
				if (m_chunkHandler->MapDeltedLLists())
				{
					m_chunkHandler->ResetMapDeletedLListsBool();
					return;
				}
				chunk::Chunk& c = jt;
				if (c.Loaded() && !c.BufferLoaded()) c.LoadGPUBuffer();
				if (c.Loaded() && c.BufferLoaded() && c.Vao() != nullptr && c.CreatedVAO())
				{
					m_renderer.UniformData(m_udata, m_chunkHandler->Locations());
					m_renderer.AInstancedRender(GL_POINTS, c.Vao(), 0, 1, c.NumBlocks());
				}
			}
		}
	}
	void Engine::RenderDebug(void)
	{
		UpdateDebugData();
		m_renderer.VecIMMRender(m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES), m_debug.Size(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES));
		m_renderer.VecIMMRender(m_debug.First(debug::Debug::option_t::RENDER_CHUNK_CORNER), m_debug.Size(debug::Debug::option_t::RENDER_CHUNK_CORNER));
	}
	void Engine::UpdateData(void)
	{
		UpdatePlayerData();
		UpdateUniformData();
	}
	void Engine::InitDebugData(void)
	{
		m_debug.Resize(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES, 3);
		m_debug.Resize(debug::Debug::option_t::RENDER_CHUNK_CORNER, 4);
	}
	void Engine::Init(void)
	{
		m_player = new ent::Player();
		m_chunkHandler = new chunk::CHandler(static_cast<int32_t>(time(NULL)));
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

		m_constantConfigs.gravity = glm::vec3(0.0f, -12.5f, 0.0f);
	}
	void Engine::UDataInit(uint32_t wwidth, uint32_t wheight)
	{
		m_chunkHandler->GetUniform();

		m_udata.projectionMatrix = glm::perspective(m_variableConfigs.FOV, (float)wwidth / wheight, 0.1f, 1000.0f);
		m_udata.lightPosition = glm::vec3(0.0f, 100.0f, 0.0f);
	}
	void Engine::AfterGLEWInit(uint32_t wwidth, uint32_t wheight,
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
		EnableDebugger();
		InitDebugData();
	}
	void Engine::RecieveKeyInput(key_t&& key)
	{
		/* obstruction handling */
		bool obstx[2];
		obstx[0] = m_chunkHandler->Obstruction(glm::vec3(1.0f, 0.0f, 0.0f), *m_player->EntityWorldPosition());
		obstx[1] = m_chunkHandler->Obstruction(glm::vec3(-1.0f, 0.0f, 0.0f), *m_player->EntityWorldPosition());
		bool obstz[2];
		obstz[0] = m_chunkHandler->Obstruction(glm::vec3(0.0f, 0.0f, 1.0f), *m_player->EntityWorldPosition());
		obstz[1] = m_chunkHandler->Obstruction(glm::vec3(0.0f, 0.0, -1.0f), *m_player->EntityWorldPosition());

		switch (key)
		{
		case key_t::R:
			m_player->SpeedUp();
			break;
		case key_t::W:
			m_player->Move(ent::Entity::move_t::FORWARD, &m_time, obstx, obstz);
			break;
		case key_t::A:
			m_player->Strafe(ent::Entity::strafe_t::LEFT, &m_time, obstx, obstz);
			break;
		case key_t::S:
			m_player->Move(ent::Entity::move_t::BACKWARD, &m_time, obstx, obstz);
			break;
		case key_t::D:
			m_player->Strafe(ent::Entity::strafe_t::RIGHT, &m_time, obstx, obstz);
			break;
		case key_t::F:
			m_player->ToggleState(ent::Entity::state_t::TOGGLE_FLY);
			break;
		case key_t::SPACE:
			m_player->Move(ent::Entity::move_t::JUMP, &m_time, obstx, obstz);
			break;
		case key_t::LSHIFT:
			m_player->Move(ent::Entity::move_t::CROUCH, &m_time, obstx, obstz);
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
		bool blockUnderPlayerPresent = m_chunkHandler->Obstruction(glm::vec3(0.0f, -1.0f, 0.0f), *m_player->EntityWorldPosition());
		if (!m_player->AttainedMaxJHeight() && m_player->Jumping())
		{
			blockUnderPlayerPresent = false;
		}
		//float blockUnderPlayer = m_chunkHandler->BlockUnderPoint(*m_player->EntityWorldPosition());
		m_player->UpdData(&m_constantConfigs.gravity, blockUnderPlayerPresent, static_cast<float>(m_time.deltaT));
	}
	void Engine::EnableDebugger(void)
	{
		m_debug.DebugMode(true);
		m_debug.Enable(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES);
		m_debug.Enable(debug::Debug::option_t::RENDER_CHUNK_CORNER);
	}

	void Engine::UpdateDebugData(void)
	{
		/* for the axes next to the player */
		auto YAxisLine = [&](void)->debug::Line { return { *m_player->EntityWorldPosition() - glm::vec3(0.0f, 3.0f, 0.0f),
			*m_player->EntityWorldPosition() + glm::vec3(0.0f, 0.01f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }; };
		auto XAxisLine = [&](void)->debug::Line { return { *m_player->EntityWorldPosition() - glm::vec3(3.0f, 0.0f, 0.0f),
			*m_player->EntityWorldPosition() + glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) }; };
		auto ZAxisLine = [&](void)->debug::Line { return { *m_player->EntityWorldPosition() - glm::vec3(0.0f, 0.0f, 3.0f),
			*m_player->EntityWorldPosition() + glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f) }; };

		m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES)[0] = YAxisLine();
		m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES)[1] = XAxisLine();
		m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES)[2] = ZAxisLine();

		/* for the chunk corners */
		glm::vec3 playerPosition = *m_player->EntityWorldPosition();
		//	chunk::ChunkDB::CCorners cc = m_chunkHandler->ChunkCorners(
		//	m_chunkHandler->CalculateCoordsInChunks(glm::vec2(playerPosition.x, playerPosition.z)));

		chunk::ChunkDB::CCorners cc = { glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f) };

		auto nn = [&](void)->debug::Line { return { glm::vec3(cc.nn.x, -50.0f, cc.nn.y) - 0.5f,
			glm::vec3(cc.nn.x, 50.0f, cc.nn.y) - 0.5f, glm::vec3(1.0f) }; };
		auto np = [&](void)->debug::Line { return { glm::vec3(cc.np.x, -50.0f, cc.np.y) - 0.5f,
			glm::vec3(cc.np.x, 50.0f, cc.np.y) - 0.5f, glm::vec3(1.0f) }; };
		auto pn = [&](void)->debug::Line { return { glm::vec3(cc.pn.x, -50.0f, cc.pn.y) - 0.5f,
			glm::vec3(cc.pn.x, 50.0f, cc.pn.y) - 0.5f, glm::vec3(1.0f) }; };
		auto pp = [&](void)->debug::Line { return { glm::vec3(cc.pp.x, -50.0f, cc.pp.y) - 0.5f,
			glm::vec3(cc.pp.x, 50.0f, cc.pp.y) - 0.5f, glm::vec3(1.0f) }; };

		m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES)[0] = nn();
		m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES)[1] = np();
		m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES)[2] = pn();
		m_debug.First(debug::Debug::option_t::RENDER_PLAYER_XYZ_AXES)[3] = pp();
	}
	void Engine::RecieveMouseInput(mbutton_t&& button)
	{
		switch (button)
		{
		case mbutton_t::MOUSEL:
			m_chunkHandler->RecieveChunkEvent(chunk::ChunkEventHandler::event_t::DIG, 
				*m_player->EntityWorldPosition(), *m_player->EntityViewDirection());
		case mbutton_t::MOUSER:
			m_chunkHandler->RecieveChunkEvent(chunk::ChunkEventHandler::event_t::PLACE,
				*m_player->EntityWorldPosition(), *m_player->EntityViewDirection());
		}
	}
}