#ifndef MESH_HEADER
#define MESH_HEADER

#include <glm/glm.hpp>

#include "../data/vertex_data.h"
#include "../data/index_data.h"
#include "../heightmap/heightmap.h"
#include "../../buffer/buffer.h"

// todo one day

struct Time;
struct UniformLocations;

class Mesh
{
public:
	struct Dimensions
	{
		float x;
		float z;
		float yMax;
	};
	struct MeshData
	{
		float* y;

		VertexData vertices;
		IndexData indices;

		unsigned int xNumVerts, zNumVerts;
		unsigned int xNumTiles, zNumTiles;
		float xHalf, zHalf;
	};

	explicit Mesh(Dimensions dim);
private:
	void SendUniformData(glm::mat4& proj, glm::mat4& view, glm::mat4& model, glm::vec3& eyePos, 
		glm::vec3& lightPos, UniformLocations* locations, Time*);
	__forceinline glm::vec2 GetPositionOfPlayerOnTile(float x, float z)
	{
		float tx = x - floor(x);
		float tz = z - ceil(z);
		return glm::vec2(tx, tz);
	}
	__forceinline glm::vec2 GetSizeOfTile(float x, float z)
	{
		float tileWidth = floor(abs(x - (m_meshData.xHalf - m_dimensions.x)));
		float tileHeight = floor(abs(z - (m_meshData.zHalf - m_dimensions.z)));
		return glm::vec2(tileWidth, tileHeight);
	}
private:
	const char* HM = "..\\robo\\res\\terrain1.png";
	Dimensions m_dimensions;
	MeshData m_meshData;
	Buffer m_buffer;
	Heightmap m_hm;
};

#endif