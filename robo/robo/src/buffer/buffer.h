#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#include "../engine/data/vertex_data.h"
#include "../engine/data/index_data.h"

class Buffer
{
public:
	explicit Buffer(void);
	explicit Buffer(VertexData vData, IndexData iData);
	void Init(VertexData vData, IndexData iData);
	void Init(void);
	void BindElement(void) const;
	void BindArray(void) const;
	void BindVertexArray(void) const;
	void BindAll(void) const;
	void DeleteBuffer(void);
	unsigned int NumIndices(void) const;
	unsigned int NumVertices(void) const;
private:
	unsigned int VertexDataSize(void) const;
	unsigned int IndexDataSize(void) const;
	void SendVertexData(void);
	void SendIndexData(void);
	void CreateVertexArray(void);
private:
	VertexData m_vData;
	IndexData m_iData;

	unsigned int m_vertexBufferID;
	unsigned int m_indexBufferID;
	unsigned int m_vaoID;
};

#endif