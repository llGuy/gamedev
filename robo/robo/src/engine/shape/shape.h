#ifndef SHAPE_HEADER
#define SHAPE_HEADER

#include <glm/glm.hpp>

#include "../data/vertex_data.h"
#include "../data/index_data.h"
#include "../../buffer/buffer.h"
#include "../entities/entity.h"

struct UniformLocations;
struct Time;

class Shape
{
public:
	virtual void Draw(Entity::UniData& ud, glm::mat4& model, UniformLocations* locations,
		Time* timeData) = 0;
	virtual void Init(void) = 0;
	virtual void DeleteBuffers(void) = 0;
protected:
	virtual void VerticesInit(void) = 0;
	virtual void IndicesInit(void) = 0;
	virtual void BufferInit(void) = 0;
	virtual void DeleteRAM(void) = 0;

	virtual void UniformData(Entity::UniData& ud, glm::mat4& model,
		Time* timeData, UniformLocations* locations) = 0;
protected:
	VertexData m_vd;
	IndexData m_id;
	Buffer m_buffer;
};

#endif