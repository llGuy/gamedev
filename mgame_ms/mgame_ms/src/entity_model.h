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
		EntityModel(float radius);

		inline
		const VAO& VertexArray(void) const
		{
		    return m_vao;
		}
		inline
		DElementsData OGLBuffer(void) 
		{
		    return { &m_ibuffer, 0, NUM_INDICES };
		}
		inline
		Buffer& VertexBuffer(void)
		{
		    return m_buffer;
		}

		inline
		float Radius(void)
		{
			return m_radius;
		}
    private:
		void CreateVertices(float radius);
		void CreateIndices(void);
		void VAOInit(void);
    private:
		static constexpr uint32_t NUM_VERTICES = 8;
		static constexpr uint32_t NUM_INDICES = 36;
		
		Buffer m_buffer;
		Buffer m_ibuffer;
		VAO m_vao;

		float m_radius;
    };

}

#endif /* _ENTITY_MODEL_H_ */

