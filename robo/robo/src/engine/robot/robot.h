#ifndef ROBOT_HEADER
#define ROBOT_HEADER

#include <glm/glm.hpp>

#include "../../buffer/buffer.h"
#include "../data/index_data.h"
#include "../data/vertex_data.h"
#include "../shape/cube.h"
#include "../entities/entity.h"

struct Time;
struct UniformLocations;
class FPSPlayer;
class Terrain;
class Gun;

class Robot final
	: public Entity
{
public:
	explicit Robot(float radius, glm::vec2 plainPosition,
		glm::vec3 color = glm::vec3(0.0f / 255.0f, 191.0f / 255.0f, 255.0f / 255.0f));		//the robot is a cube
public:
	const bool Draw(Entity::UniData& ud, UniformLocations* locations,
		Entity::DrawData& dd, Entity* player, Entity::Troops_t& troops) override;

	//new 
	void Move(const move_t&& movement, const glm::vec2& playerPlainPos) override;
	const bool DetectBulletCollision(const glm::vec3& worldCoords,
		const float& circleRad) override;
	const bool Alive(void) override;
	void DeleteBuffers(void) override;
	void RemoveLife(void) override;
	void UpdTransMat(const float& terrHeight) override;
	glm::vec2 PlainPosition(void) override;
	void Power(const power_t&& power, const glm::vec3& playerPos) override;
	glm::vec3 Position(void) override;
	void FindClosestTroop(Troops_t& troops) override;
	const bool TroopAlive(void);
	//not new 
private:
	void SendUniformData(Entity::UniData& ud, glm::mat4& model, UniformLocations* locations, Time* time);
	void RobotDataInit(void);
	const bool WantsToShoot(void);
	void Shoot(glm::vec3 playerPosition);
private:	
	Shape* m_cube;
	Gun* m_gun;
	glm::vec2 m_translateVectorPlainPosition;
	glm::vec3 m_worldCoordinates;
	glm::mat4 m_translateMatrix;
	float m_cubeRadius;
	float m_circleRadius;
	float m_robotSpeed;
	glm::vec2 m_viewDirection;
	unsigned int m_lives;
	bool m_hitPlayer;

	Entity* m_lockedTroop;
	float m_troopProximity;
	bool m_lockedOnTroop;
};

#endif