#ifndef TERRAIN_HEADER
#define TERRAIN_HEADER

#include <glm/glm.hpp>

#include "../../shader/shprogram.h"
#include "../../buffer/buffer.h"
#include "../biome/biome.h"

//#include "../engine.h"

#include "../data/vertex_data.h"
#include "../data/index_data.h"
#include "../data/time.h"
#include "../heightmap/heightmap.h"
#include "../entities/entity.h"

struct UniformLocations;
class Terrain
{
public:
	struct TerrainDimensions
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

	explicit Terrain(Terrain::TerrainDimensions, Biome::biome_t);
	~Terrain(void);

	void Draw(Entity::UniData& ud, UniformLocations*, Time*);
	glm::vec3 Sky(void);
	float GetYPosOfPlayer(float x, float z, float debug = 0.0f);
	__forceinline glm::vec2 Dimensions(void)
	{
		return glm::vec2(m_dimensions.x, m_dimensions.z);
	}
	Biome::biome_t BiomeType(void)
	{
		return m_biome->BiomeType();
	}
private:
	void TerrainInit(Biome::biome_t);
	void BufferInit(void);
	void GenerateTerrainVerts(void);
	void GenerateTerrainIndices(void);
	void InitializeHeights(void);
	void GenerateColors(void);
private:
	void SendUniformData(Entity::UniData& ud, glm::mat4& model, UniformLocations* locations, Time*);
	void InitBuffer(void);
	void InitBiome(Biome::biome_t);
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
	Terrain::TerrainDimensions m_dimensions;
	Terrain::MeshData m_meshData;
	Buffer m_buffer;
	Heightmap m_hm;
	Biome* m_biome;
};

#endif