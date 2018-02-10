#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#include "../data/vertex_data.h"
#include "../data/index_data.h"

class Buffer
{
public:
	explicit Buffer(void);
	explicit Buffer(VertexData vData, IndexData iData);
	void Init(VertexData vData, IndexData iData);
	void Init(void);
	void BindElement(void);
	void BindArray(void);
	void BindVertexArray(void);
	void BindAll(void);
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