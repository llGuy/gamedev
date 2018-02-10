#ifndef Bullet_HEADER
#define Bullet_HEADER

#include <glm/glm.hpp>
#include <vector>
#include <glm/gtx/transform.hpp>

#include "../primitives/vertex.h"
#include "../../buffer/buffer.h"
#include "../data/index_data.h"
#include "../data/vertex_data.h"
#include "../robot/robot.h"
#include "../shape/cube.h"

struct Time;
struct UniformLocations;
class FPSPlayer;

class Bullet
{
public:
	explicit Bullet(glm::vec3 playerViewDirection,
		glm::vec3 playerEyePos, float bulletSpeed = 0.3f, glm::vec3 color = glm::vec3(0.7f, 0.7f, 0.7f));
	~Bullet(void);

	void Draw(Entity::UniData& ud, UniformLocations* locations, Time* time);
	void Move(void);
	void UpdateTranslateMatrix(void);
	const bool CollisionCheck(float heightOfTerrain, std::vector<Entity*>& vec);
	const bool CollisionCheck(float heightOfTerrain, Entity* player);
	const bool CheckTerrainCollision(float heightOfTerrain);
	const bool EscapedTerrainLimits(float x, float y);
	void DeleteBuffer(void);

	glm::vec2 PlainPosition(void);
	glm::vec3 WorldCoords(void);
	const float& CircleRadius(void);
private:
	void Init(glm::vec3& playerViewDirection,
		glm::vec3 playerEyePos);
	void BulletDataInit(void);
private:
	float m_cubeRadius;			//bullet is a cube like the robot
	float m_bulletSpeed;
	glm::vec2 m_translateVectorPlainPosition;
	glm::vec3 m_worldCoordinates;
	glm::mat4 m_translateMatrix;
	glm::vec3 m_direction;
	float m_circleRadius;

	glm::vec3 m_color;

	Shape* m_cube;
};

#endif