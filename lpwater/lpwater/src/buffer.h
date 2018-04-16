#pragma once
#include <GL/glew.h>
#include <stdint.h>

class Buffer
{
public:
	Buffer(void) = default;
	~Buffer(void)
	{
		glDeleteBuffers(1, &m_bufferID);
	}
	template<typename _Ty>
	void Init(uint32_t size, _Ty* data, GLenum usage, GLenum point)
	{
		glGenBuffers(1, &m_bufferID);
		Bind(point);
		glBufferData(point, size, data, usage);
	}
	template<typename _Ty>
	void SubData(uint32_t offset, uint32_t size, _Ty* data)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
	__forceinline
		const uint32_t BufferID(void) const noexcept
	{
		return m_bufferID;
	}
	__forceinline
		void Bind(GLenum point) const
	{
		glBindBuffer(point, m_bufferID);
	}
protected:
	uint32_t m_bufferID;
};