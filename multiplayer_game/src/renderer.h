#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "buffer.h"
#include "vao.h"

namespace mulgame {

    class Renderer
    {
    public:
	Renderer(void) = default;

	void EDraw(DElementsData data, const VAO& vao, GLenum mode)
	{
	    vao.Bind();
	    
	    data.buffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
	    glDrawElements(mode, data.count, GL_UNSIGNED_SHORT, data.offset);
	    
	    vao.Unbind();
	}

	void ADraw(Buffer& buffer, uint32_t count, const VAO& vao, GLenum mode)
	{
	    buffer.Bind(GL_ARRAY_BUFFER);
	    vao.Bind();
	    glDrawArrays(mode, 0, count);
	    vao.Unbind();
	    buffer.Unbind(GL_ARRAY_BUFFER);
	}
    };

}

#endif /* _RENDERER_H_ */
