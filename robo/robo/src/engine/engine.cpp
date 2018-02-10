#include "engine.h"
#include "events/keyboard.h"

#include <GL/glew.h>
#include <ctime>
#include <cmath>

RoboEngine::RoboEngine(float windowWidth, float windowHeight, glm::vec2 cursorPos)
	: m_lighting({ glm::vec3(0.0f, 100.0f, 0.0f) }), m_configurations(),
	  m_camera(cursorPos)
{ 
	srand(static_cast<int>(time(NULL)));

	Configure();
	Configs& c = m_configurations;

	m_terrain = new Terrain({ m_configurations.terrainWidth, m_configurations.terrainDepth, 
		m_configurations.terrainMaxHeight }, Biome::VOLCANO);

	m_skyColor.currentSkyColor = m_terrain->Sky();
	m_skyColor.defaultSkyColor = m_terrain->Sky();

	m_fps = new FPSPlayer({ glm::vec3(c.originalPlayerPosition.x,
		m_terrain->GetYPosOfPlayer(c.originalPlayerPosition.x,
			c.originalPlayerPosition.z), c.originalPlayerPosition.z),
		c.originalPlayerViewDirection, c.playerSpeed, c.playerHeight, c.playerViewBobbing,
		c.playerRunningSpeedDelta});
	
	m_camera.Bind(m_fps);
	InitRobots();

	if (m_terrain->BiomeType() == Biome::VOLCANO) m_shaders = new SHProgram("..\\robo\\res\\lavaVsh.shader", 
		"..\\robo\\res\\lavaFsh.shader", "..\\robo\\res\\lavaGsh.shader");
	else m_shaders = new SHProgram("..\\robo\\res\\vsh.shader", "..\\robo\\res\\fsh.shader", "..\\robo\\res\\gsh.shader");

	MatricesInit(windowWidth, windowHeight);
	CompileShaders();
	GetUniformLocations();
	InitializeTime();
	InitDrawData();

	UpdateData();
}
RoboEngine::~RoboEngine(void)
{
	delete m_terrain;
	delete m_fps;
	delete m_shaders;
}
void RoboEngine::Configure(void)
{
	Configs& c = m_configurations;

	c.numberOfRobots = 2;

	c.playerSpeed = 0.02f;
	c.playerRunningSpeedDelta = 1.4f;
	c.playerViewBobbing = 0.002f;
	c.playerHeight = 1.0f;
	c.originalPlayerPosition = glm::vec3(0.0f);
	c.originalPlayerViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

	c.terrainWidth = 250.0f;
	c.terrainDepth = 250.0f;
	c.terrainMaxHeight = 100.0f;

	//perspective matrices
	c.renderDistance = 200.0f;
	c.fov = glm::radians(60.0f);

	//shader configs
	c.vsh = "res\\vsh.shader";
	c.fsh = "res\\fsh.shader";
	c.gsh = "res\\gsh.shader";

	m_skyColor.playerHitColor = glm::vec3(0.8f, 0.0f, 0.0f);
	m_skyColor.resurrectColor = glm::vec3(0.0f, 0.8f, 0.0f);
	m_skyColor.timeBetweenChange = 0.2f;
}
void RoboEngine::Draw(void)
{
	glClearColor(m_skyColor.currentSkyColor.r, m_skyColor.currentSkyColor.g,
		m_skyColor.currentSkyColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (AllRobotsDied()) ResurectRobots();
	UpdateMatrices();
	UpdateTimeData();
	MoveRobots();
	UpdateData();
	m_shaders->UseProgram();
	DrawAll();
	ChangeSkyColorIfColorChangedForEnoughTime();
}
void RoboEngine::UpdateData(void)
{
	m_uniformData.projection = m_transformMatrices.projection;
	m_uniformData.view = m_transformMatrices.view;
	m_uniformData.eyePosition = m_fps->Position();
	m_uniformData.lightPosition = m_lighting.lightPosition;
}
void RoboEngine::DrawAll(void)
{
	glm::vec3 playerPosition = m_fps->Position();
	if (m_fps->BulletAiring())
	{
		m_fps->DrawBullets(m_uniformData, &m_uniformLocations, m_drawData, m_robots);
	}
	m_fps->DrawTroops(m_uniformData, &m_uniformLocations, m_drawData, m_robots);
	DrawRobots(m_uniformData, &m_uniformLocations, m_drawData);
	m_terrain->Draw(m_uniformData, &m_uniformLocations, &m_timeData);
}

void RoboEngine::KeyInput(GLFWwindow* window)
{
	bool movement = false;
	if (m_fps->Running())
	{
		m_fps->NeutralizeSpeed(m_configurations.playerSpeed);
		*m_fps->Running() = false;
	}
	if (glfwGetKey(window, GLFW_KEY_R))
	{
		*m_fps->Running() = true;
		m_fps->SpeedUp();
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		m_fps->Move(Entity::move_t::FORWARD, m_terrain->GetYPosOfPlayer(m_fps->Position().x, m_fps->Position().z));
		movement = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		m_fps->Move(Entity::move_t::BACKWARD, m_terrain->GetYPosOfPlayer(m_fps->Position().x, m_fps->Position().z));
		movement = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		m_fps->Strafe(Entity::strafe_t::LEFT, m_terrain->GetYPosOfPlayer(m_fps->Position().x, m_fps->Position().z));
		movement = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		m_fps->Strafe(Entity::strafe_t::RIGHT, m_terrain->GetYPosOfPlayer(m_fps->Position().x, m_fps->Position().z));
		movement = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE)) m_fps->Power(Entity::power_t::JUMP);
	if (glfwGetKey(window, GLFW_KEY_T)) 
	{
		m_fps->Power(Entity::power_t::FLOURISH);
		m_fps->DisablePower(Entity::power_t::FLOURISH);
	}
	if (glfwGetKey(window, GLFW_KEY_B))
	{
		m_fps->Power(Entity::power_t::BOOST);
		m_fps->DisablePower(Entity::power_t::BOOST);
	}
	if (glfwGetKey(window, GLFW_KEY_F))
	{
		m_fps->Power(Entity::power_t::TROOP);
		m_fps->DisablePower(Entity::power_t::TROOP);
	}
	if (movement)
	{
		m_fps->ViewBob();
	}
}
void RoboEngine::MouseInput(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	m_camera.Look(glm::vec2(x, y));
}
void RoboEngine::MouseButtonInput(int button)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) m_fps->Power(Entity::power_t::SHOOT);
}
void RoboEngine::MatricesInit(float windowWidth, float windowHeight)
{
	m_transformMatrices.projection = glm::perspective(m_configurations.fov, 
		(float)windowWidth / windowHeight, 0.1f, m_configurations.renderDistance);
	m_transformMatrices.view = m_camera.ViewMat();
}
void RoboEngine::CompileShaders(void)
{
	std::vector<std::string> locations = {"aM_vertexPosition", "aM_vertexColor"};
	m_shaders->Compile();
	m_shaders->Link(locations);
}
void RoboEngine::GetUniformLocations(void) 
{
	m_uniformLocations.m_uniLocProjection = glGetUniformLocation(m_shaders->ProgramID(), "u_projectionMatrix");
	m_uniformLocations.m_uniLocView = glGetUniformLocation(m_shaders->ProgramID(), "u_viewMatrix");
	m_uniformLocations.m_uniLocModel = glGetUniformLocation(m_shaders->ProgramID(), "u_modelMatrix");
	m_uniformLocations.m_uniLocLightPosition = glGetUniformLocation(m_shaders->ProgramID(), "u_lightPosition");
	m_uniformLocations.m_uniLocEyePosition = glGetUniformLocation(m_shaders->ProgramID(), "u_eyePosition");
	if (m_terrain->BiomeType() == Biome::VOLCANO)
	{
		m_uniformLocations.m_uniLocTime = glGetUniformLocation(m_shaders->ProgramID(), "u_time");
		m_uniformLocations.m_uniLocLavaHeightTopPosition = glGetUniformLocation(m_shaders->ProgramID(), "u_lavaHeightTop");
	}
}
void RoboEngine::UpdateMatrices(void)
{
	m_fps->UpdData(m_terrain, &m_timeData);
	m_transformMatrices.view = m_camera.ViewMat();
}
void RoboEngine::InitializeTime(void)
{
	m_timeData.currentTime = std::chrono::high_resolution_clock::now();
	m_timeData.beginning = std::chrono::high_resolution_clock::now();
	m_skyColor.startOfChange = std::chrono::high_resolution_clock::now();
}
void RoboEngine::UpdateTimeData(void)
{
	m_timeData.deltaT = (double)((std::chrono::high_resolution_clock::now() - m_timeData.currentTime).count()) / 1000000000;
	m_timeData.currentTime = std::chrono::high_resolution_clock::now();
}
void RoboEngine::MoveRobots(void)
{
	for (auto& iter : m_robots)
	{
		float heightOfRobot = m_terrain->GetYPosOfPlayer(iter->PlainPosition().x, iter->PlainPosition().y);
		glm::vec2 playerPlainPositions = glm::vec2(m_fps->Position().x, m_fps->Position().z);
		
		if(m_fps->PlacedTroops()) 
			iter->FindClosestTroop(*m_fps->Troops());
		iter->Move(Entity::move_t::TO_PLAYER, playerPlainPositions);
		iter->UpdTransMat(heightOfRobot);
	}
}
void RoboEngine::DrawRobots(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd)
{
	for (auto& iter : m_robots)
	{
		bool changeColor = iter->Draw(ud, locations, dd, m_fps, *m_fps->Troops());
		if (changeColor)
		{
			m_skyColor.currentSkyColor = m_skyColor.playerHitColor;
			m_skyColor.startOfChange = std::chrono::high_resolution_clock::now();
		}
	}
}
void RoboEngine::InitRobots(void)
{
	for(unsigned int i = 0; i < m_configurations.numberOfRobots; ++i)
		SpawnRobot();
}
void RoboEngine::SpawnRobot(void)
{
	float x = rand() % (signed int)m_configurations.terrainWidth / 2 - m_configurations.terrainWidth / 4;
	float z = rand() % (signed int)m_configurations.terrainDepth / 2 - m_configurations.terrainDepth / 4;

	//size of robot
	float radius = static_cast<float>(rand() % 20 / 4);
	radius /= 2;
	if (radius < 0.25f) radius = 0.25f;

	m_robots.push_back(new Robot(radius, glm::vec2(x, z)));
}
const bool RoboEngine::AllRobotsDied(void)
{
	return m_robots.size() == 0;
}
void RoboEngine::ResurectRobots(void)
{
	m_skyColor.currentSkyColor = m_skyColor.resurrectColor;
	m_skyColor.startOfChange = std::chrono::high_resolution_clock::now();
	m_fps->EnablePower(Entity::power_t::FLOURISH);
	m_fps->EnablePower(Entity::power_t::BOOST);
	m_fps->EnablePower(Entity::power_t::TROOP);
	m_configurations.numberOfRobots += 2;
	std::cout << "RESURRECTED THE ROBOTS!!!" << std::endl;
	for (unsigned int i = 0; i < m_configurations.numberOfRobots; ++i)
		SpawnRobot();
}
void RoboEngine::ChangeSkyColorIfColorChangedForEnoughTime(void)
{
	double duration = (double)((std::chrono::high_resolution_clock::now() - m_skyColor.startOfChange).count()) / 1000000000;
	if (duration > m_skyColor.timeBetweenChange)
		m_skyColor.currentSkyColor = m_skyColor.defaultSkyColor;
}
void RoboEngine::InitDrawData(void)
{
	m_drawData.terrain = m_terrain;
	m_drawData.timeData = &m_timeData;
}