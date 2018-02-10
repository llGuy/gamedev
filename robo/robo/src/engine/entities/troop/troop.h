#ifndef TROOP_HEADER
#define TROOP_HEADER

#include "../../shape/cube.h"
#include "../entity.h"

//player summoned robot that fights other robots

class Gun;

class Troop final
	: public Entity
{
public:
	explicit Troop(float radius, glm::vec3 plainPosition,
		glm::vec3 color = glm::vec3(255.0f / 255.0f, 80.0f / 255.0f, 0.0f / 255.0f));		//the robot is a cube
public:
	const bool Draw(Entity::UniData& ud, UniformLocations* locations,
		Entity::DrawData& dd, Entity::Robots_t& robots) override;

	const bool DetectBulletCollision(const glm::vec3& worldCoords,
		const float& circleRad) override;
	const bool Alive(void) override;
	const bool BulletAiring(void) override;
	void DeleteBuffers(void) override;
	void RemoveLife(void) override;
	glm::vec3 Position(void) override;
	glm::vec2 PlainPosition(void) override;
	void Power(const power_t&& power, const std::vector<Entity*>& robots) override;
	void DrawBullets(Entity::UniData& ud, UniformLocations* locations, Entity::DrawData& dd, Entity::Robots_t& vec) override;
private:
	void SendUniformData(Entity::UniData& ud, glm::mat4& model,  UniformLocations* locations, Time* time);
	void TroopDataInit(void);
	const bool WantsToShoot(void);
	void Shoot(const std::vector<Entity*>& robots);
private:
	Shape* m_cube;
	Gun* m_gun;
	glm::vec3 m_worldCoordinates;
	glm::mat4 m_translateMatrix;
	float m_cubeRadius;
	float m_circleRadius;
	float m_yElevation;
	unsigned int m_lives;
};

#endif