#ifndef CUBE_HEADER
#define CUBE_HEADER

#include "shape.h"

struct UniformLocations;
struct Time;

class Cube 
	: public Shape
{
public:
	explicit Cube(float rad, glm::vec3 color, const bool elevate);
	void Draw(Entity::UniData& ud, glm::mat4& model,
		UniformLocations* locations, Time* timeData) override;
	void Init(void) override;
	void DeleteBuffers(void) override;
protected:
	void VerticesInit(void) override;
	void IndicesInit(void) override;
	void BufferInit(void) override;
	void DeleteRAM(void) override;

	void UniformData(Entity::UniData& ud, glm::mat4& model,
		Time* timeData, UniformLocations* locations) override;
protected:
	glm::vec3 m_color;
	float m_cubeRadius;
	bool m_elevate;
};

#endif