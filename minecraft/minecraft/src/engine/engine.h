#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <unordered_map>
#include <vector>

#include "chunk/handler/chandler.h"
#include "../shader/shprogram.h"
#include "chunk/chunk.h"
#include "renderer/renderer.h"
#include "chunk/map/cmap.h"
#include "../shader/shprogram.h"
#include "configs/configs.h"
#include "debug/debug.h"
#include "entities/player/player.h"
#include "entities/camera/camera.h"
#include "../texture/texture.h"
#include "gui/handler/guihandler.h"
#include "texture_atlas/texture_atlas.h"

namespace minecraft
{
	/* Minecraft engine which will render everything */
	class Engine
	{
	public:
		enum class mbutton_t
		{
			MOUSEL,

			MOUSER
		};
		enum class key_t
			: int32_t
		{
			W,				// forward
			
			A,				// left
			
			S,				// backward
			
			D,				// right

			R,				// run

			F,				// toggle fly
			
			SPACE,			// jump
			
			LSHIFT,			// crouch

			ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE	// for the hotbar
		};
		explicit Engine(void);
		~Engine(void);
		void AfterGLEWInit(uint32_t wwidth, uint32_t wheight, 
			glm::vec2 cursorPos, GLFWwindow* window);
	public:
		void RecieveMouseInput(mbutton_t&& button);
		void RecieveKeyInput(const key_t& key);
		void RecieveMouseMovement(glm::vec2 newMousePosition);
		void Render(void);
		void HEAPDelete(void);
	private:
		void Init(void);
		void TimeDataInit(void);
		void Configure(void);
		void UDataInit(uint32_t wwidth, uint32_t wheight);
		void UpdateUniformData(void);
		void UpdateData(void);
		void UpdateDebugData(void);
		void EnableDebugger(void);
		void InitDebugData(void);
		void UpdatePlayerData(void);
		void RenderChunks(void);
		void RenderDebug(void);
		void RenderGUI(void);
	private:
		//Texture m_textureAtlas;
		TextureAtlas m_blockTextureAtlas;
		ent::Camera m_camera;
		ent::Entity* m_player;
		gui::GUIHandler m_guihandler;
		data::Time m_time;
		data::CUData m_udata;
		rnd::Renderer m_renderer;
		debug::Debug m_debug;
		configs::VConfigs m_variableConfigs;
		configs::CConfigs m_constantConfigs;
		chunk::CHandler* m_chunkHandler;
		float m_fps;
	};
}

#endif
