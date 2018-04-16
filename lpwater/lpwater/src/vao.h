#pragma once
#include <stdint.h>
#include <GL/glew.h>

class VAO
{
public:
	VAO(void) = default;
	~VAO(void)
	{
		glDeleteVertexArrays(1, &m_vaoID);
	}
	void Init(void)
	{
		glGenVertexArrays(1, &m_vaoID);
		Bind();
	}
	void EnableArray(uint32_t loc)
	{
		glEnableVertexAttribArray(loc);
	}
	void VertexAttribPtr(uint32_t index, uint32_t size, GLenum type, uint32_t stride, void* ptr)
	{
		glVertexAttribPointer(index, size, type, /*never normalized*/ GL_FALSE, stride, ptr);
	}
	void Divisor(uint32_t index, uint32_t divisor)
	{
		glVertexAttribDivisor(index, divisor);
	}
	__forceinline
		void Bind(void) const
	{
		glBindVertexArray(m_vaoID);
	}
	__forceinline
		void UnBind(void) const
	{
		glBindVertexArray(0);
	}
	__forceinline
		const uint32_t VAOID(void) const
	{
		return m_vaoID;
	}
protected:
	uint32_t m_vaoID;
};