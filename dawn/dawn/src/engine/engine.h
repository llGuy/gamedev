#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <iostream>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include "mesh/mesh.h"
#include "shader/program/udata.h"
#include "shader/program/program.h"
#include "renderer/renderer.h"

namespace dawn {

	class DawnEngine
	{
	public:
		// version number
		DawnEngine(const int32_t& width, const int32_t& height) noexcept;
		DawnEngine(const DawnEngine&) = delete;
		const DawnEngine& operator=(const DawnEngine&) = delete;

		void Init(void);
	public:
		void Render(void);
	private:
		void ShadersInit(void);
		void UdataInit(void);
	private:
		// testing
		DawnMesh m_mesh;
		SHProgram m_program;
		Renderer m_renderer;
	};

}

#endif