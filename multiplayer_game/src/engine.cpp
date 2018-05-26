#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include "engine.h"

namespace mulgame {

    MULGEngine::MULGEngine(int32_t width, int32_t height)
	: m_entityProgram{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, 
	  m_terrainProgram{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }
    {
	Configure();
	InitData(width, height);
	InitShaders();
	InitEntities();
	InitTerrain();
	InitOpenglGLStates();
    }

    void MULGEngine::Configure(void)
    {
	m_configs.fov = glm::radians(60.0f);
	m_configs.near = 0.1f;
	m_configs.far = 1000.0f;
    }

    void MULGEngine::Render(void)
    {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_entityProgram.UseProgram();
		
	RenderEntities();
	RenderTerrain();
	RenderBullets();
    }

    void MULGEngine::Update(void)
    {
	m_ehandler.Update(m_terrain);
    }
    
    void MULGEngine::RenderEntities(void)
    {
	m_entityProgram.UseProgram();

	Camera& camera = m_ehandler.Cam();
	EntityModel& model = m_ehandler.EModel();
	for(auto entity = m_ehandler.ebegin(); entity != m_ehandler.eend(); ++entity)
	{
	    Entity* entPtr = &(*entity);
	    Entity* boundEntPtr = &m_ehandler[camera.BoundEntity()];
	    // don't draw the entity that the camera is bound to
	    if(entPtr != boundEntPtr)
	    {
		glm::vec3 color { 1.0f, 0.0f, 0.0f };
		glm::mat4 translation = glm::translate(entity->Eye());
		glm::mat4 view = camera.ViewMatrix(m_ehandler[camera.BoundEntity()]);
		glm::mat4& projection = m_data.matProjection;
		m_entityProgram.UniformData(&color[0], &translation[0][0], &view[0][0], &projection[0][0]);
		m_renderer.EDraw(model.OGLBuffer(), model.VertexArray(), GL_TRIANGLES);
	    }
	}
    }

    void MULGEngine::InitData(int32_t width, int32_t height)
    {
	float aspect = static_cast<float>(width) / height;
	m_data.matProjection = glm::perspective(m_configs.fov, aspect, m_configs.near, m_configs.far);
	m_data.lightPosition = glm::vec3(-20.0f, 15.0f, -20.0f);
    }

    void MULGEngine::InitShaders(void)
    {
	InitEntityShaders();
	InitTerrainShaders();
    }

    void MULGEngine::InitEntityShaders(void)
    {
	m_entityProgram.Compile("../res/entity/vsh.shader", "../res/entity/fsh.shader");
	m_entityProgram.Link("vertex_position");

	m_entityProgram.GetUniformLocations
	    (
		UDataLoc(udata_t::VEC3, "model_color"),
		UDataLoc(udata_t::MAT4, "matrix_model"),
		UDataLoc(udata_t::MAT4, "matrix_view"),
		UDataLoc(udata_t::MAT4, "matrix_projection")
		);
    }

    void MULGEngine::InitTerrainShaders(void)
    {
	m_terrainProgram.Compile("../res/terrain/vsh.shader", "../res/terrain/fsh.shader");
	m_terrainProgram.Link("vertex_color");

	m_terrainProgram.GetUniformLocations
	    (
		UDataLoc(udata_t::VEC3, "color"),
		UDataLoc(udata_t::MAT4, "matrix_view"),
		UDataLoc(udata_t::MAT4, "matrix_projection"),
		UDataLoc(udata_t::VEC3, "light_position")
	    );
    }

    void MULGEngine::InitEntities(void)
    {
	Entity& ent = m_ehandler.PushEntity(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.1f, 0.01f, 1.0f));
	// binding the camera to the player (first entity)
	m_ehandler.BindCamera(0);
	m_ehandler.PushEntity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.01f, 0.01f));
	m_ehandler.StartTimer();
    }

    void MULGEngine::InitTerrain(void)
    {
	m_terrain.Init();
    }

    void MULGEngine::InitOpenglGLStates(void) const
    {
	glEnable(GL_DEPTH_TEST);
    }

    void MULGEngine::RenderTerrain(void)
    {
	m_terrainProgram.UseProgram();
	Camera& camera = m_ehandler.Cam();
     	glm::mat4 view = camera.ViewMatrix(m_ehandler[camera.BoundEntity()]);
	glm::mat4& projection = m_data.matProjection;
	glm::vec3 color(0.4f, 0.4f, 0.4f);
	glm::vec3 lightPosition(0.0f, 10.0f, 0.0f);

	m_terrainProgram.UniformData(&color[0], &view[0][0], &projection[0][0], &lightPosition[0]);
		
	DElementsData data = m_terrain.OGLBuffer();
	m_renderer.EDraw(data, m_terrain.VertexArray(), GL_TRIANGLES);
    }

    void MULGEngine::RenderBullets(void)
    {
	m_entityProgram.UseProgram();
	Camera& camera = m_ehandler.Cam();
	EntityModel& model = m_ehandler.BModel();
	for (auto bullet = m_ehandler.bbegin(); bullet != m_ehandler.bend(); ++bullet)
	{
	    glm::vec3 color{ 0.2f, 0.2f, 0.2f };
	    glm::mat4 translation = glm::translate(bullet->pos);
	    glm::mat4 view = camera.ViewMatrix(m_ehandler[camera.BoundEntity()]);
	    glm::mat4& projection = m_data.matProjection;
	    m_entityProgram.UniformData(&color[0], &translation[0][0], &view[0][0], &projection[0][0]);
	    m_renderer.EDraw(model.OGLBuffer(), model.VertexArray(), GL_TRIANGLES);
	}
    }

}
