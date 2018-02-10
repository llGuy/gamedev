#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "engine_comon.h"
#include "configs/configs.h"
#include "../camera/camera.h"
#include "entities/entity.h"

#include <chrono>
#include <vector>

struct UniformLocations
{
	signed int m_uniLocModel;
	signed int m_uniLocProjection;
	signed int m_uniLocView;
	signed int m_uniLocLightPosition;
	signed int m_uniLocEyePosition;
	signed int m_uniLocTime;
	signed int m_uniLocLavaHeightTopPosition;
};

class RoboEngine
{
public:
	struct TransformMatrices
	{
		glm::mat4 projection;
		glm::mat4 view;
	};
	struct Lighting
	{
		glm::vec3 lightPosition;
	};
	struct SkyColor
	{
		glm::vec3 resurrectColor;
		glm::vec3 playerHitColor;
		glm::vec3 defaultSkyColor;
		glm::vec3 currentSkyColor;
		std::chrono::high_resolution_clock::time_point startOfChange;
		float timeBetweenChange;
	};

	explicit RoboEngine(float windowWidth, 
		float windowHeight, glm::vec2 mousePosition);
	~RoboEngine(void);
	
	void Draw(void);
	void KeyInput(GLFWwindow*);
	void MouseInput(GLFWwindow*);
	void MouseButtonInput(int button);
private:
	void UpdateData(void);
	void Configure(void);
	void MatricesInit(float windowWidth, float windowHeight);
	void CompileShaders(void);
	void GetUniformLocations(void);
	void UpdateMatrices(void);
	void InitializeTime(void);
	void UpdateTimeData(void);
	void MoveRobots(void);
	void DrawRobots(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd);
	void DrawAll(void);
	void InitRobots(void);
	void SpawnRobot(void);
	const bool AllRobotsDied(void);
	void ResurectRobots(void);
	void ChangeSkyColorIfColorChangedForEnoughTime(void);
	void InitDrawData(void);
private:
	Configs m_configurations;
	Time m_timeData;
	Terrain* m_terrain;
	SHProgram* m_shaders;
	SkyColor m_skyColor;
	Lighting m_lighting;
	Camera m_camera;
	UniformLocations m_uniformLocations;
	TransformMatrices m_transformMatrices;

	Entity::Player_t m_fps; 
	Entity::Robots_t m_robots;
	Entity::UniData m_uniformData;
	Entity::DrawData m_drawData;
};

#endif