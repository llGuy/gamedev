#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <GL/glew.h>
#include <iostream>

namespace mulgame {

    class Buffer
    {
    public:
	Buffer(void)
	{
	    glGenBuffers(1, &m_bufferID);
	}
	~Buffer(void)
	{
	    glDeleteBuffers(1, &m_bufferID);
	}
	
	void Bind(GLenum point) const
	{
	    glBindBuffer(point, m_bufferID);
	}

	void Unbind(GLenum point) const
	{
	    glBindBuffer(point, 0);
	}	
	
	template<typename _Ty>
	void Fill(uint32_t size, _Ty* data, GLenum usage, GLenum point)
	{
	    Bind(point);
	    glBufferData(point, size, data, usage);
	}

	template<typename _Ty>
	void PartialFill(uint32_t offset, uint32_t size, _Ty* data, GLenum point)
	{
	    Bind(point);
	    glBufferSubData(point, offset, size, data);
	}
	
	inline
	uint32_t& ID(void) 
        {
	    return m_bufferID;
	}
    private:
	uint32_t m_bufferID;
    };

    struct DElementsData
    {
		Buffer* buffer;
		void* offset;
		uint32_t count;
    };

}

#endif /* _BUFFER_H_ */
