#ifndef CONFIG_HEADER
#define CONFIG_HEADER

struct Configs
{
	unsigned int numberOfRobots;

	float playerSpeed;
	float playerRunningSpeedDelta;
	float playerViewBobbing;
	float playerHeight ;
	glm::vec3 originalPlayerPosition;
	glm::vec3 originalPlayerViewDirection;

	float terrainWidth;
	float terrainDepth;
	float terrainMaxHeight;

	//perspective matrices
	float renderDistance;
	float fov;

	//shader configs
	const char* vsh ;
	const char* fsh;
	const char* gsh;
};

#endif