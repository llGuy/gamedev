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
	InitEntities();
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
	
/*	EntityModel& model = m_ehandler.Model();
	glm::vec3 color { 1.0f, 0.0f, 0.0f };
	glm::mat4 translation = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4& projection = m_data.matProjection;
	m_genericProgram.UniformData(&color[0], &translation[0][0], &view[0][0], &projection[0][0]);
	
	m_renderer.EDraw(model.OGLBuffer(), model.VertexArray(), GL_TRIANGLES);*/
	RenderEntities();
    }

    void MULGEngine::Update(void)
    {
	m_ehandler.Update();
    }
    
    void MULGEngine::RenderEntities(void)
    {
	Camera& camera = m_ehandler.Cam();
	EntityModel& model = m_ehandler.Model();
	for(auto entity : m_ehandler)
	{
	    // don't draw the entity that the camera is bound to
	    if(&entity != camera.BoundEntity())
	    {
		// test color
		glm::vec3 color { 1.0f, 0.0f, 0.0f };
		glm::mat4 translation = glm::translate(entity.Position());
		glm::mat4& view = camera.ViewMatrix();
		glm::mat4& projection = m_data.matProjection;

		m_genericProgram.UniformData(&color[0], &translation[0][0], &view[0][0], &projection[0][0]);
		m_renderer.EDraw(model.OGLBuffer(), model.VertexArray(), GL_TRIANGLES);
	    }
	}
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
	Entity& ent = m_ehandler.PushEntity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.01f, 0.01f));
	// binding the camera to the player (first entity)
	Camera& camera = m_ehandler.Cam();
	camera.BoundEntity() = &m_ehandler[0];

	m_ehandler.PushEntity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.01f, 0.01f));
    }
    
}
