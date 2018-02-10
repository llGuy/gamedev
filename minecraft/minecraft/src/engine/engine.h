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
#include "entities/player/player.h"
#include "entities/camera/camera.h"
#include "../texture/texture.h"

namespace minecraft
{
	/* Minecraft engine which will render everything */
	class Engine
	{
	public:
		enum class key_t
		{
			W,				// forward
			
			A,				// left
			
			S,				// backward
			
			D,				// right
			
			SPACE,			// jump
			
			LSHIFT			// crouch
		};
		explicit Engine(void);
		void AfterGLEWInit(unsigned int wwidth, unsigned int wheight, 
			glm::vec2 cursorPos, GLFWwindow* window);
	public:
		void RecieveKeyInput(key_t&& key);
		void RecieveMouseMovement(glm::vec2 newMousePosition);
		void Render(void);
	private:
		void Init(void);
		void TimeDataInit(void);
		void Configure(void);
		void UDataInit(unsigned int wwidth, unsigned int wheight);
		void UpdateUniformData(void);
	private:
		Texture m_textureAtlas;
		ent::Camera m_camera;
		ent::Entity* m_player;
		data::Time m_time;
		data::CUData m_udata;
		rnd::Renderer m_renderer;
		configs::VConfigs m_variableConfigs;
		configs::CConfigs m_constantConfigs;
		chunk::CHandler* m_chunkHandler;
		float m_fps;
	};
}

#endif
