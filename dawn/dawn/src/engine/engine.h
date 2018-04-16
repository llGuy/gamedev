#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <iostream>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include "mesh/mesh.h"
#include "terrain/chunk/chunk_handler.h"
#include "shader/program/udata.h"
#include "shader/program/program.h"
#include "renderer/renderer.h"
#include "entities/camera/camera.h"
#include "entities/player/player.h"

namespace dawn {

	enum class action_t
	{
		W, A, S, D, 

		SPACE, LSHIFT,

		INV
	};

	class DawnEngine
	{
	public:
		DawnEngine(int32_t width, int32_t height);
		DawnEngine(const DawnEngine&) = delete;
		const DawnEngine& operator=(const DawnEngine&) = delete;

		void Init(const glm::vec2& cursorPos);
	public:
		void Render(void);
		void RecieveAction(action_t a);
		void MouseCursor(const glm::vec2&);
	private:
		void ShadersInit(void);
		void UdataInit(void);
		void CameraInit(const glm::vec2& cursor);
		void TerrainInit(void);
	private:
		glm::mat4 m_projectionMatrix;

		SHProgram m_program;
		Renderer m_renderer;
		terrain::chunk::ChunkHandler m_chandler;

		ent::Entity* m_player;
		ent::Camera m_camera;
	};

}

#endif