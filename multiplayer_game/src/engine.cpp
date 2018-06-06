#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>

#include "engine.h"

namespace mulgame {

    MULGEngine::MULGEngine(int32_t width, int32_t height, int8_t arg)
	: m_entityProgram{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER },
	  m_terrainProgram{ GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER },
	  m_lflareProgram{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER },
	  m_lighting(glm::vec3(100.0f, 100.0f, 100.0f)),
	  m_networkHandler(arg == 's' ? mode_t::SERVER_MODE : mode_t::CLIENT_MODE, m_ehandler, m_terrain)
    {
	Configure();
	InitData(width, height);
	InitShaders();
	InitEntities();
	InitTerrain();
	InitNetworkHandler();
	InitOpenglGLStates();
    }

    void MULGEngine::InitNetworkHandler(void)
    {
	if(m_networkHandler.Mode() == SERVER_MODE)
	{
	    m_networkHandler.Launch("5000", m_ehandler, m_terrain);
	}
	else
	{
	    m_networkHandler.Launch("192.168.1.230", "5000", m_ehandler, m_terrain);
	}
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
	if(m_networkHandler.Mode() == mode_t::CLIENT_MODE)
	{
	    // receive
	    
	    auto received = m_networkHandler.ReceiveFromServer();
	    if(received.has_value())
	    {
		MSGParser& parser = received.value();
		for(; !parser.Max();)
		{
		    m_networkHandler.ParseUDPMessage(m_ehandler, m_terrain, parser);
		}
	    }
	    if(m_networkHandler.ServerReturnedWithMessage())
	    {
		MSGEncoder encoder;
		Entity& player = m_ehandler[m_ehandler.Cam().BoundEntity()];
		encoder.PushString(player.Username());
		encoder.PushBytes(player.Position(), player.Direction());

		// push if player is shooting and terraforming
		bool boundEntityShot = m_ehandler.BoundEntityShot();

		bool boundEntityTerraformed = (player.Terraforming() != -1);

		// 0b0000 0000 <- shot
		//          ^ terraformed
		int8_t flags = boundEntityShot + (boundEntityTerraformed << 1);

		encoder.PushBytes(flags);
		if(boundEntityTerraformed)
		{
		    decltype(auto) fp = m_terrain.FP(player.Terraforming());
		    // push force point
		    encoder.PushBytes(fp);
		}
		else encoder.PushBytes(ForcePoint{});
	    
		m_networkHandler.SendPlayerDatatoServer(encoder.Vector(), encoder.Size());
		m_networkHandler.ServerReturnedWithMessage() = false;
	    }
	}
	
	m_ehandler.Update(m_terrain);
	m_terrain.UpdateForcePoints(m_ehandler.Timedelta());
    }

    void MULGEngine::RenderEntities(void)
    {
	m_entityProgram.UseProgram();

	Camera& camera = m_ehandler.Cam();
	EntityModel& model = m_ehandler.EModel();
	for (auto entity = m_ehandler.ebegin(); entity != m_ehandler.eend(); ++entity)
	{
	    Entity* entPtr = &(*entity);
	    Entity* boundEntPtr = &m_ehandler[camera.BoundEntity()];
	    // don't draw the entity that the camera is bound to
	    if (entPtr != boundEntPtr)
	    {
		glm::vec3 color{ 1.0f, 0.0f, 0.0f };
		glm::mat4 translation = glm::translate(entity->Eye());

//		glm::vec3& dir = entity->Direction();
//		glm::vec3 axes = glm::vec3(0.0f, dir.x, 0.0f);
		
//		glm::mat4 rotation = glm::rotate(glm::radians(90.0f), axes);
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
	InitLFlareShaders();
    }


    void MULGEngine::InitEntities(void)
    {
	Entity& ent = m_ehandler.PushEntity(glm::vec3(0.0f, 0.0f, -0.01f), glm::vec3(1.0f, 0.0f, 0.01f));
	// binding the camera to the player (first entity)
	m_ehandler.BindCamera(0);
	if(m_networkHandler.Mode() == mode_t::SERVER_MODE) m_ehandler[0].Username() = "server";

	m_ehandler.StartTimer();
    }



    std::string MULGEngine::ShaderPath(const std::string & shaderName) const
    {
#ifdef _WIN32
	return "res/" + shaderName;
#else
	return "../res/" + shaderName;
#endif
    }

    void MULGEngine::InitEntityShaders(void)
    {
	m_entityProgram.Compile(ShaderPath("entity/vsh.shader"), ShaderPath("entity/fsh.shader"));
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
	m_terrainProgram.Compile(ShaderPath("terrain/vsh.shader"), ShaderPath("terrain/gsh.shader"), ShaderPath("terrain/fsh.shader"));
	m_terrainProgram.Link("vertex_position");

	m_terrainProgram.GetUniformLocations
	    (
		UDataLoc(udata_t::VEC3, "color"),
		UDataLoc(udata_t::MAT4, "matrix_view"),
		UDataLoc(udata_t::MAT4, "matrix_projection"),
		UDataLoc(udata_t::VEC3, "light_position")
		);
    }

    void MULGEngine::InitLFlareShaders(void)
    {
	m_lflareProgram.Compile(ShaderPath("lensflare/shader/vsh.shader"), ShaderPath("lensflare/shader/fsh.shader"));
	m_lflareProgram.Link("vertex_position", "texture_coordinate");

	m_lflareProgram.GetUniformLocations
	    (
		UDataLoc(udata_t::MAT4, "matrix_projection"),
		UDataLoc(udata_t::VEC2, "translation"),
		UDataLoc(udata_t::F1, "scale"),
		UDataLoc(udata_t::F1, "brightness")
		);
    }

    void MULGEngine::InitTerrain(void)
    {
	m_terrain.Init();
    }

    void MULGEngine::InitOpenglGLStates(void) const
    {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    void MULGEngine::RenderTerrain(void)
    {
	m_terrainProgram.UseProgram();
	Camera& camera = m_ehandler.Cam();
	glm::mat4 view = camera.ViewMatrix(m_ehandler[camera.BoundEntity()]);
	glm::mat4& projection = m_data.matProjection;
	glm::vec3 color(0.8f, 0.4f, 0.0f);
	glm::vec3& lightPosition = m_lighting.LightPosition();

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

    void MULGEngine::RenderLFlare(void)
    {
	Camera& camera = m_ehandler.Cam();
	Entity& bound = m_ehandler[camera.BoundEntity()];
	m_lensFlareHandler.UpdatePositions(m_lighting.LightPosition(), camera.ViewMatrix(bound), m_data.matProjection);

	if (m_lensFlareHandler.Visible())
	{
	    glDisable(GL_DEPTH_TEST);
	    m_lflareProgram.UseProgram();
	    glm::mat4& projection = m_data.matProjection;
	    for (uint32_t i = 0; i < LFlareHandler::NUM_FLARE_TEXTURES; ++i)
	    {
		Texture& tex = m_lensFlareHandler.Tex(i);
		VAO& vao = m_lensFlareHandler.VertexArray();
		glm::vec2& translation = m_lensFlareHandler.Position(i);
		float scale = m_lensFlareHandler.Scale(i);
		float brightness = m_lensFlareHandler.Brightness();

		tex.Bind(0);
		m_lflareProgram.UniformData(&projection[0][0], &translation[0], &scale, &brightness);
		m_renderer.ADraw(m_lensFlareHandler.OGLBuffer(), 4, vao, GL_TRIANGLE_STRIP);
	    }
	}
	glEnable(GL_DEPTH_TEST);
    }

}
