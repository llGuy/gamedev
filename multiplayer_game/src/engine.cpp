#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include "engine.h"

namespace mulgame {

    MULGEngine::MULGEngine(int32_t width, int32_t height)
	: m_genericProgram{GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}
    {
	Configure();
	InitData(width, height);
	InitShaders();
    }

    void MULGEngine::Configure(void)
    {
	m_configs.fov = glm::radians(60.0f);
	m_configs.near = 0.1f;
	m_configs.far = 100.0f;
    }

    void MULGEngine::Render(void)
    {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

	m_genericProgram.UseProgram();

	// testing rectangle
/*	glm::vec3 color{ 0.0f, 1.0f, 0.0f };
	glm::mat4 m(1.0f);
	glm::mat4 v(1.0f);
	glm::mat4 p(1.0f);*/
	
	EntityModel& model = m_ehandler.Model();
	m_renderer.EDraw(model.OGLBuffer(), model.VertexArray(), GL_TRIANGLES);
//	m_renderer.ADraw(model.VertexBuffer(), 3, model.VertexArray());
    }

    void MULGEngine::InitData(int32_t width, int32_t height)
    {
	float aspect = static_cast<float>(width) / height;
	m_data.matProjection = glm::perspective(m_configs.fov, aspect, m_configs.near, m_configs.far);
    }

    void MULGEngine::InitShaders(void)
    {
	m_genericProgram.Compile("../res/vsh.shader", "../res/fsh.shader");
	m_genericProgram.Link("vertex_position");

	m_genericProgram.GetUniformLocations
	(
	    UDataLoc( udata_t::VEC3, "model_color" ),
	    UDataLoc( udata_t::MAT4, "matrix_model" ),
	    UDataLoc( udata_t::MAT4, "matrix_view" ),
	    UDataLoc( udata_t::MAT4, "matrix_projection")
	);
    }

    void MULGEngine::InitEntities(void)
    {
//	m_ehandler.PushEntity(glm::vec3());
    }
    
}
