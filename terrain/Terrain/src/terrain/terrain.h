#ifndef TERRAIN_HEADER
#define TERRAIN_HEADER

#include <glm/glm.hpp>

#include "../shader/shprogram.h"
#include "../buffer/buffer.h"
#include "../biome/biome.h"

/*

struct Vertex;
class Heightmap;
class Biome;

class Terrain
{
public:
	explicit Terrain(float dimRow, float dimCol, float, Biome::biome_t);
	~Terrain(void);

	void Draw(glm::mat4& mvp, glm::mat4& viewMat, glm::vec3& eyePos);
	glm::vec3 Sky(void);
	float GetYPosOfPlayer(float x, float z, float debug = 0.0f);
private:
	void TerrainInit(Biome::biome_t);
	void BufferAndShaderInit(void);
	void GenerateTerrainVerts(void);
	void GenerateTerrainIndices(void);
	void InitializeHeights(void);
	void GenerateColors(void);
private:
	void CompileShaders(void);
	void InitHeightmap(void);
	void GetUniformLocations(void);
	void SendUniformData(glm::mat4& proj, glm::mat4& view, glm::mat4& model, glm::vec3& eyePos);
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
		float tileWidth = floor(abs(x - (m_widthHalf - m_width)));
		float tileHeight = floor(abs(z - (m_heightHalf - m_height)));
		return glm::vec2(tileWidth, tileHeight);
	}
private:
	struct UniformLocations
	{
		signed int m_uniLocModel;
		signed int m_uniLocProjection;
		signed int m_uniLocView;
		signed int m_uniLocLightPosition;
		signed int m_uniLocEyePosition;
	} m_uniformLocations;

	typedef unsigned int uint32;
	typedef unsigned char uint8;
	typedef unsigned short uint16;

	//dimRow
	float m_width;					// dimentions
	float m_height;

	VertexData m_vData;
	IndexData m_iData;

	glm::vec3 m_lightPosition;

	float* m_yVals;
	float m_maxHeight;

	unsigned int m_numVertsWidth;
	unsigned int m_numVertsHeight;

	unsigned int m_numTilesWidth;
	unsigned int m_numTilesHeight;

	float m_widthHalf;
	float m_heightHalf;

	SHProgram m_shprogram;
	Buffer* m_buffer;
	Heightmap* m_hm;
	Biome* m_biome;
};

*/
#include "../heightmap/heightmap.h"
#include "../data/vertex_data.h"
#include "../data/index_data.h"


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
	struct UniformLocations
	{
		signed int m_uniLocModel;
		signed int m_uniLocProjection;
		signed int m_uniLocView;
		signed int m_uniLocLightPosition;
		signed int m_uniLocEyePosition;
	};

	explicit Terrain(Terrain::TerrainDimensions, Biome::biome_t);
	~Terrain(void);

	void Draw(glm::mat4& mvp, glm::mat4& viewMat, glm::vec3& eyePos);
	glm::vec3 Sky(void);
	float GetYPosOfPlayer(float x, float z, float debug = 0.0f);
private:
	void TerrainInit(Biome::biome_t);
	void BufferAndShaderInit(void);
	void GenerateTerrainVerts(void);
	void GenerateTerrainIndices(void);
	void InitializeHeights(void);
	void GenerateColors(void);
private:
	void CompileShaders(void);
	void InitHeightmap(void);
	void GetUniformLocations(void);
	void SendUniformData(glm::mat4& proj, glm::mat4& view, glm::mat4& model, glm::vec3& eyePos);
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
	Terrain::UniformLocations m_uniformLocations;
	Terrain::TerrainDimensions m_dimensions;
	Terrain::MeshData m_meshData;
	SHProgram m_shprogram;
	Buffer m_buffer;
	Heightmap m_hm;
	Biome* m_biome;
	glm::vec3 m_lightPosition;
};

#endif