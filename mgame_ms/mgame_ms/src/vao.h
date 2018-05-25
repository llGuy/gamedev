#ifndef _VAO_H_
#define _VAO_H_

#include <iostream>
#include <GL/glew.h>

namespace mulgame {

    class VAO
    {
    public:
	VAO(void)
	{
	    glGenVertexArrays(1, &m_vaoID);
	}

	void Bind(void) const
	{
	    glBindVertexArray(m_vaoID);
	}

	void Unbind(void) const
	{
	    glBindVertexArray(0);
	}

	void Enable(uint32_t loc)
	{
	    glEnableVertexAttribArray(loc);
	}

	void VAPtr(uint32_t index, uint32_t size, GLenum type, uint32_t stride, void* ptr)
	{
	    glVertexAttribPointer(index, size, type, GL_FALSE /*never normalized*/, stride, ptr);
	}

	void Divisor(uint32_t index, uint32_t div)
	{
	    glVertexAttribDivisor(index, div);
	}

	uint32_t ID(void) const
	{
	    return m_vaoID;
	}
    private:
	uint32_t m_vaoID;
    };

}

#endif /* _VAO_H_ */
