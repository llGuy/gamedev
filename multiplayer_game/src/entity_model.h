#ifndef _ENTITY_MODEL_H_
#define _ENTITY_MODEL_H_

#include <glm/glm.hpp>
#include <memory>
#include "vao.h"
#include "buffer.h"

namespace mulgame {

    // entity model is a cube
    class EntityModel
    {
    public:
	EntityModel(void);

	inline
	const VAO& VertexArray(void) const
	{
	    return m_vao;
	}
	inline
	DElementsData OGLBuffer(void) 
	{
	    return { &m_ibuffer, 0, 3 };
	}
	inline
	Buffer& VertexBuffer(void)
	{
	    return m_buffer;
	}
    private:
	void CreateVertices(void);
	void CreateIndices(void);
	void VAOInit(void);
    private:
	static constexpr uint32_t NUM_VERTICES = 3;
	static constexpr uint32_t NUM_INDICES = 3;
	
	Buffer m_buffer;
	Buffer m_ibuffer;
	VAO m_vao;
    };

}

#endif /* _ENTITY_MODEL_H_ */

