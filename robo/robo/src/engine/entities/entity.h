#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <vector>
#include <glm/glm.hpp>

struct Time;
struct UniformLocations;

class Terrain;
class FPSPlayer;

class Entity
{
public:
	// the uniform data needed for an entity to be drawn
	struct UniData
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec3 eyePosition;
		glm::vec3 lightPosition;
	};
	// draw data needed for an entity to be drawn
	struct DrawData
	{
		Time* timeData;
		Terrain* terrain;
	};

	/*	the player moves forward or backward
		the robot only moves towards the player */
	enum class move_t
	{
		FORWARD,

		BACKWARD,

		TO_PLAYER
	};
	/*	for the player	*/
	enum class strafe_t
	{
		RIGHT,

		LEFT
	};
	/*	for the player who has powers like flourish or boost...	*/
	enum class power_t
	{
		FLOURISH,

		BOOST,

		JUMP,

		TROOP,

		SHOOT
	};

	typedef Entity* Player_t;
	typedef Entity* Troop_t;
	typedef std::vector<Entity*> Robots_t;
	typedef std::vector<Entity*> Troops_t;

	virtual Troops_t* Troops(void) { return nullptr; }
	virtual const bool PlacedTroops(void) { return false; }
	virtual glm::vec2 PlainPosition(void) { return glm::vec2(); }
	virtual glm::vec3* ViewDirection(void) { return nullptr; }
	virtual glm::vec3 Position(void) { return glm::vec3(); }
	virtual const glm::mat4 ViewMatrix(Terrain* terr, Time* time) { return glm::mat4(); }
	virtual void UpdTransMat(const float& terrHeight) {}
	virtual void UpdData(Terrain* terrain, Time* time) {}

	virtual const bool Draw(UniData& ud, UniformLocations* ul, DrawData& dd, Player_t player, Troops_t& troops) { return false; }
	virtual const bool Draw(UniData& ud, UniformLocations* ul, DrawData& dd, Robots_t& robots) { return false; }
	virtual void DrawBullets(UniData& ud, UniformLocations* ul, DrawData& dd, Robots_t& vec) {}
	virtual void DrawBullets(UniData& ud, UniformLocations* ul, DrawData& dd, Player_t*) {} 
	virtual void DrawTroops(UniData& ud, UniformLocations* locations, DrawData& drawData, Robots_t& robots) {}
	virtual void FindClosestTroop(Troops_t& troops) {}

	virtual void Move(const move_t&& movement, const float& terrHeight,
		const glm::vec3& direction = glm::vec3(0.0f)) {}
	virtual void Move(const move_t&& movement, const glm::vec2& playerPlainPos) {}
	virtual void Strafe(const strafe_t&& strafe, const float& terrHeight) {}
	virtual void Power(const power_t&& power, const glm::vec3& playerPos = glm::vec3(0.0f)) {}
	virtual void Power(const power_t&& power, const std::vector<Entity*>& robots) {}
	virtual void DisablePower(const power_t&& power) {}
	virtual void EnablePower(const power_t&& power) {}

	// for the bullets
	virtual const bool DetectBulletCollision(const glm::vec3& worldCoords, 
		const float& circleRad) { return false; }
	virtual const bool Alive(void) { return false; }
	virtual void DeleteBuffers(void) {}
	virtual void RemoveLife(void) {}

	// for the player
	virtual bool* Running(void) { return nullptr; }
	virtual void SpeedUp(void) {}
	virtual void NeutralizeSpeed(const float& speed) {}
	virtual const bool BulletAiring(void) { return false; }
	virtual void ViewBob(void) {}
};

#endif